#ifndef OMNIBOAT_R_INCLUDED
#define OMNIBOAT_R_INCLUDED

#include <array>

#include "mbed.h"

#include "MPU6050.h"

constexpr float schneider_PI = 3.141592653589;

namespace omniboat {

constexpr float I = 1;
constexpr float e = 0.01;
constexpr float a = 0.1;
constexpr int trial_num = 1000;  // 試行回数

class Schneider {
public:
    Schneider();
    ~Schneider();
    void debug();
    void one_step();
    void init();
    void flip_shneider();
    void led(int num);  // 点灯

private:
    float phi;                  // 機体の姿勢
    std::array<float, 3> gyro;  // ジャイロ入力
    void ticker_flip();

    std::array<std::array<float, 3>, 4> t_jacobianmatrix;  // ヤコビ行列
    std::array<float, 3> x_d;  // ジョイスティックからの入力
    std::array<float, 4> q;    // 更新される入力
    std::array<float, 3> x;    // qに対しての出力
    float volume_;

    void cal_tjacob();      // 転置したヤコビ行列の計算
    void state_equation();  // 機体の状態の計算
    void cal_q();           // 入力の推定と更新
    void set_q();           // モーターへの入力
    void joy_read(float joy_x, float joy_y, int rotate);
    void rotate();

private:
    AnalogIn adcIn1;  // ジョイスティック
    AnalogIn adcIn2;  // ジョイスティック
    AnalogIn volume;
    MPU6050 mpu;

    PwmOut servo_1;
    PwmOut servo_2;
    PwmOut fet_1;
    PwmOut fet_2;

    DigitalOut led1;
    DigitalOut led2;
    DigitalOut led3;

    Ticker flip;

    DigitalIn button;
    BufferedSerial pc;
};
}  // namespace omniboat

#endif
