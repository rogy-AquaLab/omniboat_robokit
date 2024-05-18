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

Schneider::Schneider() :
    inputs(),
    outputs(),
    last_output({0, 0}, {0, 0}),
    input_modules({A4, A5}, A6, {D4, D5}),
    output_modules({PB_4, PA_11}, {PA_9, PA_10}) {
    trace::toggle(LedId::First);
    trace::toggle(LedId::Second);
    trace::toggle(LedId::Third);
    printf("start up\n");
    this->output_modules.init();
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
    const bool whoami = this->input_modules.mpu_whoami();
    if (whoami) {
        printf("WHOAMI succeeded\n");
    } else {
        printf("WHOAMI failed\n");
    }
}

inline auto map_joy(const std::pair<float, float>& joy) -> std::array<float, 3> {
    // ジョイスティックの中心値
    constexpr float joy_center = 0.5F;
    // [2]はrot
    return {(joy.first - joy_center) * 2, (joy.second - joy_center) * 2, 0};
}

void Schneider::one_step() {
    using std::abs;
    // ジョイスティックの値の実効下限値
    constexpr float joy_min = 0.4F;
    // つまみの値の実効範囲 (x < volume_under || volume_over < x で有効)
    constexpr float volume_under = 0.4F;
    constexpr float volume_over = 0.7F;

    trace::toggle(LedId::Third);

    const packet::InputValues input = this->input_modules.read();
    const std::array<float, 3> joy = map_joy(input.joy);

    this->inputs[0] = 0;
    this->inputs[1] = 0;

    const bool joyEffective = abs(joy[0]) > joy_min && abs(joy[1]) > joy_min;
    const bool volumeEffective = input.volume < volume_under || volume_over < input.volume;

    packet::OutputValues output;
    if (joyEffective) {
        this->cal_q(joy);
        output = this->set_q(input.gyro);
    } else if (volumeEffective) {
        output = this->rotate(input.volume);
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
    // 3.64 ~= 2200 * PI / 1900
    // ea3f45b の src/schneider_model.cpp:238 と同様の計算式になるように
    // TODO: 係数調整
    constexpr float gyro_coef = 3.64F;

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
        servo_output.first = this->inputs[2] - gyro_coef * gyro[2];
    }
    if (0 < this->inputs[3] && this->inputs[3] < PI) {
        servo_output.second = this->inputs[3] + gyro_coef * gyro[2];
    }
    const packet::OutputValues output(servo_output, fet_output);
    return output;
}

// NOLINTBEGIN(readability-convert-member-functions-to-static)
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
// NOLINTEND(readability-convert-member-functions-to-static)

auto Schneider::stop_fet() const -> packet::OutputValues {
    packet::OutputValues output(this->last_output.servo, {0, 0});
    return output;
}

auto Schneider::write_output(const packet::OutputValues& output) -> void {
    if (!output.is_valid()) {
        // TODO: outputの値を出力する
        printf("ERROR: received invalid OutputValues\n");
        return;
    }
    this->last_output = output;
    this->output_modules.write(output);
}

}  // namespace omniboat
