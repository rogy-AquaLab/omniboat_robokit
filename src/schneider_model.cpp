#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <thread>

#include "rtos.h"

#include "schneider_model.hpp"

namespace omniboat {

Schneider::Schneider() :
    q(),
    x(),
    adcIn1(A4),
    adcIn2(A5),
    volume(A6),
    mpu(D4, D5),
    servo_1(PB_4),
    servo_2(PA_11),
    fet_1(PA_9),
    fet_2(PA_10),
    led1(PA_1),
    led2(PA_3),
    led3(PA_4),
    pc(USBTX, USBRX) {
    this->led(1);
    this->led(2);
    this->led(3);
    printf("start up\n");

    this->servo_1.period_ms(pwmPeriodMs);
    this->servo_2.period_ms(pwmPeriodMs);
}

Schneider::~Schneider() {
    this->led(1);
}

void Schneider::init() {
    using std::fill;
    constexpr float initialQ = 0.01F;
    constexpr float initialX = 0.0F;

    fill(this->q.begin(), this->q.end(), initialQ);
    fill(this->x.begin(), this->x.end(), initialX);
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
    this->led(3);

    // ジャイロセンサの値を読み取る
    const auto gyro = this->read_gyro();

    // ジョイコンの値を読み取り、目標値を代入
    const auto joy = this->read_joy();

    // ボリュームの値を読み取る
    const float volume_value = volume.read();

    this->q[0] = 0;
    this->q[1] = 0;

    const bool joyEffective = abs(joy[0]) > joyThreshold || abs(joy[1]) > joyThreshold;
    const bool volumeEffective = volume_value < volumeIneffectiveRange.first
                                 || volumeIneffectiveRange.second < volume_value;

    if (joyEffective) {
        this->cal_q(joy);
        this->set_q(gyro);
    } else if (volumeEffective) {
        this->rotate(volume_value);
    } else {
        this->fet_1 = 0;
        this->fet_2 = 0;
    }
    this->debug();
    this->led(3);
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
    const auto joy_x = this->adcIn1.read();
    const auto joy_y = this->adcIn2.read();

    const auto dest_x = (joy_x - joyCenter) * 2;
    const auto dest_y = (joy_y - joyCenter) * 2;
    const auto dest_rot = 0.0F;
    return {dest_x, dest_y, dest_rot};
}

inline std::array<std::array<float, 3>, 4> Schneider::cal_tjacob() const {
    using std::cos;
    using std::sin;
    std::array<std::array<float, 3>, 4> t_jacobianmatrix;
    t_jacobianmatrix[0][0] = cos(this->q[2]);
    t_jacobianmatrix[0][1] = sin(this->q[2]);
    t_jacobianmatrix[0][2] = (a + sin(this->q[2])) / I;
    t_jacobianmatrix[1][0] = cos(this->q[3]);
    t_jacobianmatrix[1][1] = sin(this->q[3]);
    t_jacobianmatrix[1][2] = (-a - sin(this->q[3])) / I;
    t_jacobianmatrix[2][0] = -this->q[0] * sin(this->q[2]);
    t_jacobianmatrix[2][1] = this->q[0] * cos(this->q[2]);
    t_jacobianmatrix[2][2] = this->q[0] * cos(this->q[2]) / I;
    t_jacobianmatrix[3][0] = -this->q[1] * sin(this->q[3]);
    t_jacobianmatrix[3][1] = this->q[1] * cos(this->q[3]);
    t_jacobianmatrix[3][2] = -this->q[1] * cos(this->q[3]) / I;
    return t_jacobianmatrix;
}

auto Schneider::cal_q(const std::array<float, 3>& joy) -> void {
    using std::pow;
    // 初期値
    const float coef = (joy[0] >= 0 && joy[1] >= 0)  ? 1
                       : (joy[0] >= 0 && joy[1] < 0) ? -1
                       : (joy[0] < 0 && joy[1] >= 0) ? 3
                                                     : 5;
    for (int i = 2; i < 4; ++i) {
        this->q[i] = coef * schneider_PI / 4;
    }

    led(2);
    for (int i = 0; i < trial_num; i++) {
        this->state_equation();

        double diff = pow(this->x[0] - joy[0], 2) + pow(this->x[1] - joy[1], 2)
                      + pow(this->x[2] - joy[2], 2);
        if (diff < diffThreshold) {
            break;
        }

        const std::array<std::array<float, 3>, 4> t_jacobianmatrix = this->cal_tjacob();
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 3; k++) {
                this->q[j] -= e * t_jacobianmatrix[j][k] * (this->x[k] - joy[k]);
            }
            if (j == 0 || j == 1) {
                // 0.5に近づくように7次関数でバイアスをかけてる。多分。
                constexpr int biasOrder = 7;
                this->q[j] -= pow(2 * this->q[j] - 1, biasOrder);
            }
        }
    }
    this->led(2);
}

inline void Schneider::state_equation() {
    using std::cos;
    using std::sin;
    this->x[0] = this->q[0] * cos(this->q[2]) + this->q[1] * cos(this->q[3]);
    this->x[1] = this->q[0] * sin(this->q[2]) + this->q[1] * sin(this->q[3]);
    this->x[2] = (a * (this->q[0] - this->q[1]) + this->q[0] * sin(this->q[2])
                  - this->q[1] * sin(this->q[3]))
                 / I;
}

void Schneider::set_q(const std::array<float, 3>& gyro) {
    using std::abs;
    if (abs(this->q[0] <= joyThreshold)) {
        this->q[0] = 0;
    }
    if (abs(this->q[1] <= joyThreshold)) {
        this->q[1] = 0;
    }
    this->fet_1 = this->q[0];
    this->fet_2 = this->q[1];

    while (this->q[2] >= schneider_PI) {
        this->q[2] -= 2 * schneider_PI;
    }
    while (this->q[3] >= schneider_PI) {
        this->q[3] -= 2 * schneider_PI;
    }
    while (this->q[2] < -schneider_PI) {
        this->q[2] += 2 * schneider_PI;
    }
    while (this->q[3] < -schneider_PI) {
        this->q[3] += 2 * schneider_PI;
    }

    if (0 < this->q[2] && this->q[2] < schneider_PI) {
        const int width = static_cast<int>(500 + 1900 / schneider_PI * this->q[2] - 2200 * gyro[2]);
        this->servo_1.pulsewidth_us(width);
    }
    if (0 < this->q[3] && this->q[3] < schneider_PI) {
        const int width = static_cast<int>(500 + 1900 / schneider_PI * this->q[3] + 2200 * gyro[2]);
        this->servo_2.pulsewidth_us(width);
    }
}

void Schneider::rotate(const float& volume_value) {
    this->fet_1 = fetDuty;
    this->fet_2 = fetDuty;
    // ifとelseで内容が同じだといわれたがそんなことない
    if (volume_value < volumeThreshold) {
        this->servo_1.pulsewidth_us(minorRotatePulsewidthUs);
        this->servo_2.pulsewidth_us(majorRotatePulsewidthUs);
    } else {
        this->servo_2.pulsewidth_us(minorRotatePulsewidthUs);
        this->servo_1.pulsewidth_us(majorRotatePulsewidthUs);
    }
}

auto Schneider::read_gyro() -> std::array<float, 3> {
    std::array<float, 3> gyro;
    this->mpu.getGyro(gyro.data());
    return gyro;
}

void Schneider::led(int num) {
    // 3つの連続した明示的なブランチがあるといわれたがそんなことない
    switch (num) {
    case 1: this->led1 = !this->led1; break;
    case 2: this->led2 = !this->led2; break;
    case 3: this->led3 = !this->led3; break;
    default: break;
    }
}

}  // namespace omniboat
