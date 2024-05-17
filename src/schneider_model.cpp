#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <thread>
#include <utility>

#include "rtos.h"

#include "schneider_model.hpp"
#include "trace.hpp"
#include "utils.hpp"

namespace omniboat {

using trace::LedId;
using utils::PI;

/**
 * @brief z軸周りの慣性モーメント
 * @note もっと正確な値の方がいいかも
 */
constexpr float inertia_z = 1;
/**
 * @brief ステップ幅
 */
constexpr float step_width_a = 0.1;

// サーボモータ出力値(pulse width)
// FIXME: もともと550, 2350だったので要検証
constexpr int minor_pulsewidth_us = 500;
constexpr int major_pulsewidth_us = 2400;

Schneider::Schneider() :
    inputs(),
    outputs(),
    last_output({0, 0}, {0, 0}),
    adcIn1(A4),
    adcIn2(A5),
    volume(A6),
    mpu(D4, D5),
    servo_1(PB_4),
    servo_2(PA_11),
    fet_1(PA_9),
    fet_2(PA_10) {
    constexpr uint8_t servo_pwm_period_ms = 20U;

    trace::toggle(LedId::First);
    trace::toggle(LedId::Second);
    trace::toggle(LedId::Third);
    printf("start up\n");

    this->servo_1.period_ms(servo_pwm_period_ms);
    this->servo_2.period_ms(servo_pwm_period_ms);
}

Schneider::~Schneider() {
    trace::toggle(LedId::First);
}

void Schneider::init() {
    using std::fill;
    constexpr float initialInputs = 0.01F;
    constexpr float initialOutputs = 0.0F;

    fill(this->inputs.begin(), this->inputs.end(), initialInputs);
    fill(this->outputs.begin(), this->outputs.end(), initialOutputs);
    this->cal_tjacob();
    const bool whoami = this->mpu.testConnection();
    if (whoami) {
        printf("WHOAMI succeeded\n");
    } else {
        printf("WHOAMI failed\n");
    }
}

void Schneider::one_step() {
    using std::abs;
    // ジョイスティックの値の実効下限値
    constexpr float joy_min = 0.4F;
    // つまみの値の実効範囲 (x < volume_under || volume_over < x で有効)
    constexpr float volume_under = 0.4F;
    constexpr float volume_over = 0.7F;

    trace::toggle(LedId::Third);

    // ジャイロセンサの値を読み取る
    const auto gyro = this->read_gyro();

    // ジョイコンの値を読み取り、目標値を代入
    const auto joy = this->read_joy();

    // ボリュームの値を読み取る
    const float volume_value = volume.read();

    this->inputs[0] = 0;
    this->inputs[1] = 0;

    const bool joyEffective = abs(joy[0]) > joy_min && abs(joy[1]) > joy_min;
    const bool volumeEffective = volume_value < volume_under || volume_over < volume_value;

    packet::OutputValues output;
    if (joyEffective) {
        this->cal_q(joy);
        output = this->set_q(gyro);
    } else if (volumeEffective) {
        output = this->rotate(volume_value);
    } else {
        output = this->stop_fet();
    }
    this->write_output(output);
    this->debug();
    trace::toggle(LedId::Third);
}

void Schneider::debug() {
    // printf("ave=%f\n",ave_[0]/ave_[1]);
    // printf("motor=%f,%f\n",q[0],q[1]);
    // printf("servo=%f,%f\n",q[2],q[3]);
    // printf("volume=%f\n",volume.read());
    // printf("gyro[2]=%f\n",gyro[2]);
    // printf("\n");
}

auto Schneider::read_joy() -> std::array<float, 3> {
    // ジョイスティックの中心値
    constexpr float joy_center = 0.5F;

    const auto joy_x = this->adcIn1.read();
    const auto joy_y = this->adcIn2.read();

    const auto dest_x = (joy_x - joy_center) * 2;
    const auto dest_y = (joy_y - joy_center) * 2;
    const auto dest_rot = 0.0F;
    return {dest_x, dest_y, dest_rot};
}

inline std::array<std::array<float, 3>, 4> Schneider::cal_tjacob() const {
    using std::cos;
    using std::sin;
    std::array<std::array<float, 3>, 4> t_jacobianmatrix;
    t_jacobianmatrix[0][0] = cos(this->inputs[2]);
    t_jacobianmatrix[0][1] = sin(this->inputs[2]);
    t_jacobianmatrix[0][2] = (step_width_a + sin(this->inputs[2])) / inertia_z;
    t_jacobianmatrix[1][0] = cos(this->inputs[3]);
    t_jacobianmatrix[1][1] = sin(this->inputs[3]);
    t_jacobianmatrix[1][2] = (-step_width_a - sin(this->inputs[3])) / inertia_z;
    t_jacobianmatrix[2][0] = -this->inputs[0] * sin(this->inputs[2]);
    t_jacobianmatrix[2][1] = this->inputs[0] * cos(this->inputs[2]);
    t_jacobianmatrix[2][2] = this->inputs[0] * cos(this->inputs[2]) / inertia_z;
    t_jacobianmatrix[3][0] = -this->inputs[1] * sin(this->inputs[3]);
    t_jacobianmatrix[3][1] = this->inputs[1] * cos(this->inputs[3]);
    t_jacobianmatrix[3][2] = -this->inputs[1] * cos(this->inputs[3]) / inertia_z;
    return t_jacobianmatrix;
}

auto Schneider::cal_q(const std::array<float, 3>& joy) -> void {
    using std::pow;
    // ステップ幅
    constexpr float step_width_e = 0.01;
    // 試行回数
    constexpr size_t trial_num = 1000U;

    // 初期値
    const float coef = (joy[0] >= 0 && joy[1] >= 0)  ? 1
                       : (joy[0] >= 0 && joy[1] < 0) ? -1
                       : (joy[0] < 0 && joy[1] >= 0) ? 3
                                                     : 5;
    for (int i = 2; i < 4; ++i) {
        this->inputs[i] = coef * PI / 4;
    }

    trace::toggle(LedId::Second);
    for (size_t i = 0; i < trial_num; i++) {
        // 目標値との差の2乗ノルム(diff)の実効下限値
        constexpr float diff_min = 0.001F;

        this->state_equation();

        double diff = pow(this->outputs[0] - joy[0], 2) + pow(this->outputs[1] - joy[1], 2)
                      + pow(this->outputs[2] - joy[2], 2);
        if (diff < diff_min) {
            break;
        }

        const std::array<std::array<float, 3>, 4> t_jacobianmatrix = this->cal_tjacob();
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 3; k++) {
                this->inputs[j]
                    -= step_width_e * t_jacobianmatrix[j][k] * (this->outputs[k] - joy[k]);
            }
            if (j == 0 || j == 1) {
                // 0.5に近づくように7次関数でバイアスをかけてる。多分。
                constexpr int biasOrder = 7;
                this->inputs[j] -= pow(2 * this->inputs[j] - 1, biasOrder);
            }
        }
    }
    trace::toggle(LedId::Second);
}

inline void Schneider::state_equation() {
    using std::cos;
    using std::sin;
    this->outputs[0]
        = this->inputs[0] * cos(this->inputs[2]) + this->inputs[1] * cos(this->inputs[3]);
    this->outputs[1]
        = this->inputs[0] * sin(this->inputs[2]) + this->inputs[1] * sin(this->inputs[3]);
    this->outputs[2]
        = (step_width_a * (this->inputs[0] - this->inputs[1])
           + this->inputs[0] * sin(this->inputs[2]) - this->inputs[1] * sin(this->inputs[3]))
          / inertia_z;
}

auto Schneider::set_q(const std::array<float, 3>& gyro) -> packet::OutputValues {
    using std::abs;
    using float_pair = std::pair<float, float>;
    // 系への入力値の実効下限値
    constexpr float input_min = 0.4F;

    if (abs(this->inputs[0]) <= input_min) {
        this->inputs[0] = 0;
    }
    if (abs(this->inputs[1]) <= input_min) {
        this->inputs[1] = 0;
    }
    const float_pair fet_output = {this->inputs[0], this->inputs[1]};

    while (this->inputs[2] >= PI) {
        this->inputs[2] -= 2 * PI;
    }
    while (this->inputs[3] >= PI) {
        this->inputs[3] -= 2 * PI;
    }
    while (this->inputs[2] < -PI) {
        this->inputs[2] += 2 * PI;
    }
    while (this->inputs[3] < -PI) {
        this->inputs[3] += 2 * PI;
    }

    float_pair servo_output = this->last_output.servo;
    if (0 < this->inputs[2] && this->inputs[2] < PI) {
        // 3.64 ~= 2200 * PI / 1900
        // ea3f45b の src/schneider_model.cpp:238 と同様の計算式になるように
        // TODO: gyroの係数調整
        servo_output.first = this->inputs[2] - 3.64 * gyro[2];
    }
    if (0 < this->inputs[3] && this->inputs[3] < PI) {
        servo_output.second = this->inputs[3] + 3.64 * gyro[2];
    }
    const packet::OutputValues output(servo_output, fet_output);
    return output;
}

auto Schneider::rotate(const float& volume_value) const -> packet::OutputValues {
    using float_pair = std::pair<float, float>;
    // volumeのしきい値
    constexpr float volume_threshold = 0.5F;
    // DCモータ出力値(duty比)
    constexpr float fet_duty = 0.5F;

    const float_pair servo_output
        = volume_value < volume_threshold ? float_pair{0, PI} : float_pair{PI, 0};
    const packet::OutputValues output(servo_output, {fet_duty, fet_duty});
    return output;
}

auto Schneider::stop_fet() const -> packet::OutputValues {
    packet::OutputValues output(this->last_output.servo, {0, 0});
    return output;
}

auto Schneider::read_gyro() -> std::array<float, 3> {
    std::array<float, 3> gyro;
    this->mpu.getGyro(gyro.data());
    return gyro;
}

/// ラジアン → PWM pulsewidth_us
constexpr auto servo_value_map(float radian) -> int {
    return static_cast<int>(
        (major_pulsewidth_us - minor_pulsewidth_us) * (radian / PI) + minor_pulsewidth_us);
}

auto Schneider::write_output(const packet::OutputValues& output) -> void {
    if (!output.is_valid()) {
        // TODO: outputの値を出力する
        printf("ERROR: received invalid OutputValues\n");
        return;
    }
    this->last_output = output;
    this->servo_1.pulsewidth_us(servo_value_map(output.servo.first));
    this->servo_2.pulsewidth_us(servo_value_map(output.servo.second));
    this->fet_1.write(output.dc_motor.first);
    this->fet_2.write(output.dc_motor.second);
}

}  // namespace omniboat
