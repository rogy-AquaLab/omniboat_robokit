#ifndef OMNIBOAT_R_INCLUDED
#define OMNIBOAT_R_INCLUDED

#include <array>

#include "mbed.h"

#include "MPU6050.h"

constexpr float schneider_PI = 3.141592653589;

namespace omniboat {

/** z軸周りの慣性モーメント @note もっと正確な値の方がいいかも*/
constexpr float I = 1;
/** ステップ幅 */
constexpr float e = 0.01;
constexpr float a = 0.1;
/** 試行回数 */
constexpr int trial_num = 1000;

/**
 * @brief モータへの出力を計算するクラス
 */
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
    /** 機体の姿勢 */
    float phi;                  

    /** ジャイロセンサの値 */ 
    std::array<float, 3> gyro;

    void ticker_flip();

    /** ヤコビ行列*/
    std::array<std::array<float, 3>, 4> t_jacobianmatrix;  

    /** ジョイスティックからの入力(目標値) */
    std::array<float, 3> x_d;
    
    /** 入力値 */
    std::array<float, 4> q;    

    /** qに対しての出力 */
    std::array<float, 3> x;

    /** つまみの入力値 */
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
    MPU6050 mpu;      // 慣性計測ユニット(imu)

    PwmOut servo_1;   // servo
    PwmOut servo_2;   // servo
    PwmOut fet_1;     // DC
    PwmOut fet_2;     // DC

    DigitalOut led1;
    DigitalOut led2;
    DigitalOut led3;

    Ticker flip;

    DigitalIn button;
    BufferedSerial pc;
};
}  // namespace omniboat

#endif
