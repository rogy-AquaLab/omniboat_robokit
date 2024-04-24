#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <thread>

#include "rtos.h"

#include "schneider_model.hpp"

namespace omniboat {
// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init)
Schneider::Schneider() :
    phi(0),
    t_jacobianmatrix(),
    x_d(),
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
// NOLINTEND(cppcoreguidelines-pro-type-member-init)

Schneider::~Schneider() {
    this->led(1);
}

void Schneider::init() {
    using std::fill;
    constexpr float initialQ = 0.01F;
    constexpr float initialX = 0.0F;
    constexpr float initialPhi = 0.0F;

    this->phi = initialPhi;
    for (auto& row : this->t_jacobianmatrix) {
        fill(row.begin(), row.end(), 0);
    }
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
    auto gyro = std::array<float, 3>();
    this->mpu.getGyro(gyro.data());

    // ジョイコンの値を読み取る
    this->joy_read(this->adcIn1.read(), this->adcIn2.read(), 0.0);

    this->volume_ = this->volume.read();

    this->q[0] = 0;
    this->q[1] = 0;

    const bool joyEffective = abs(this->x_d[0]) > joyThreshold || abs(this->x_d[1]) > joyThreshold;
    const bool volumeEffective = this->volume_ < volumeIneffectiveRange.first
                                 || volumeIneffectiveRange.second < this->volume_;

    if (joyEffective) {
        this->cal_q();
        this->set_q(gyro);
    } else if (volumeEffective) {
        this->rotate();
        this->phi = 0;
    } else {
        this->fet_1 = 0;
        this->fet_2 = 0;
        this->phi = 0;
    }
    this->debug();
    this->led(3);
}

void Schneider::debug() {
    // printf("phi=%f\n",phi);
    // printf("ave=%f\n",ave_[0]/ave_[1]);
    // printf("motor=%f,%f\n",q[0],q[1]);
    // printf("servo=%f,%f\n",q[2],q[3]);
    // printf("volume=%f\n",volume.read());
    // printf("gyro[2]=%f\n",gyro[2]);
    // printf("\n");
}

void Schneider::joy_read(float joy_x, float joy_y, int rotate) {
    this->x_d[0] = (joy_x - joyCenter) * 2;
    this->x_d[1] = (joy_y - joyCenter) * 2;
    // x_d[2] = rotate;
    this->x_d[2] = 0;
}

inline void Schneider::cal_tjacob() {
    using std::cos;
    using std::sin;
    this->t_jacobianmatrix[0][0] = cos(this->q[2] + this->phi);
    this->t_jacobianmatrix[0][1] = sin(this->q[2] + this->phi);
    this->t_jacobianmatrix[0][2] = (a + sin(this->q[2])) / I;
    this->t_jacobianmatrix[1][0] = cos(this->q[3] + this->phi);
    this->t_jacobianmatrix[1][1] = sin(this->q[3] + this->phi);
    this->t_jacobianmatrix[1][2] = (-a - sin(this->q[3])) / I;
    this->t_jacobianmatrix[2][0] = -this->q[0] * sin(this->q[2] + this->phi);
    this->t_jacobianmatrix[2][1] = this->q[0] * cos(this->q[2] + this->phi);
    this->t_jacobianmatrix[2][2] = this->q[0] * cos(this->q[2]) / I;
    this->t_jacobianmatrix[3][0] = -this->q[1] * sin(this->q[3] + this->phi);
    this->t_jacobianmatrix[3][1] = this->q[1] * cos(this->q[3] + this->phi);
    this->t_jacobianmatrix[3][2] = -this->q[1] * cos(this->q[3]) / I;
}

void Schneider::cal_q() {
    using std::pow;
    // 初期値
    const float coef = (this->x_d[0] >= 0 && this->x_d[1] >= 0)  ? 1
                       : (this->x_d[0] >= 0 && this->x_d[1] < 0) ? -1
                       : (this->x_d[0] < 0 && this->x_d[1] >= 0) ? 3
                                                                 : 5;
    for (int i = 2; i < 4; ++i) {
        this->q[i] = coef * schneider_PI / 4 - this->phi;
    }

    led(2);
    for (int i = 0; i < trial_num; i++) {
        this->state_equation();

        double diff = pow(this->x[0] - this->x_d[0], 2) + pow(this->x[1] - this->x_d[1], 2)
                      + pow(this->x[2] - this->x_d[2], 2);
        if (diff < diffThreshold) {
            break;
        }

        this->cal_tjacob();
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 3; k++) {
                this->q[j] -= e * this->t_jacobianmatrix[j][k] * (this->x[k] - this->x_d[k]);
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
    this->x[0] = this->q[0] * cos(this->q[2] + phi) + this->q[1] * cos(this->q[3] + this->phi);
    this->x[1] = this->q[0] * sin(this->q[2] + phi) + this->q[1] * sin(this->q[3] + this->phi);
    this->x[2] = (a * (this->q[0] - this->q[1]) + this->q[0] * sin(this->q[2])
                  - this->q[1] * sin(this->q[3]))
                 / I;
}

void Schneider::set_q(std::array<float, 3> gyro) {
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

void Schneider::rotate() {
    this->fet_1 = fetDuty;
    this->fet_2 = fetDuty;
    // ifとelseで内容が同じだといわれたがそんなことない
    // NOLINTBEGIN(bugprone-branch-clone)
    if (this->volume_ < volumeThreshold) {
        this->servo_1.pulsewidth_us(minorRotatePulsewidthUs);
        this->servo_2.pulsewidth_us(majorRotatePulsewidthUs);
    } else {
        this->servo_2.pulsewidth_us(minorRotatePulsewidthUs);
        this->servo_1.pulsewidth_us(majorRotatePulsewidthUs);
    }
    // NOLINTEND(bugprone-branch-clone)
}

void Schneider::led(int num) {
    // 3つの連続した明示的なブランチがあるといわれたがそんなことない
    // NOLINTBEGIN(bugprone-branch-clone)
    switch (num) {
    case 1: this->led1 = !this->led1; break;
    case 2: this->led2 = !this->led2; break;
    case 3: this->led3 = !this->led3; break;
    default: break;
    }
    // NOLINTEND(bugprone-branch-clone)
}

}  // namespace omniboat
