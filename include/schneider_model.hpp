#ifndef OMNIBOAT_R_INCLUDED
#define OMNIBOAT_R_INCLUDED

#include <array>

#include "mbed.h"

#include "MPU6050.h"

constexpr float schneider_PI = 3.141592653589;

namespace omniboat {

/**
 * @brief z軸周りの慣性モーメント
 * @note もっと正確な値の方がいいかも
 */
constexpr float I = 1;  // NOLINT: FIXME
/**
 * @brief ステップ幅
 */
constexpr float e = 0.01;  // NOLINT: FIXME
constexpr float a = 0.1;   // NOLINT: FIXME

/**
 * @brief 試行回数
 */
constexpr int trial_num = 1000;

/**
 * @brief volumeの閾値
 */
constexpr float volumeThreshold = 0.5F;

/**
 * @brief ジョイスティックの下限値
 */
constexpr float joyThreshold = 0.4F;

/**
 * @brief ジョイスティックの中央値
 */
constexpr float joyCenter = 0.5F;

/**
 * @brief volumeの下限値・上限値
 */
constexpr std::pair<float, float> volumeIneffectiveRange = {0.4F, 0.7F};

/**
 * @brief pulsewidthの小さいほうの値
 */
constexpr int minorRotatePulsewidthUs = 550;

/**
 * @brief pulsewidthの大きいほうの値
 */
constexpr int majorRotatePulsewidthUs = 2350;

/**
 * @brief 座標・姿勢の目標値と現在値の偏差の有効範囲の最小値 (それ未満は偏差0とみなす)
 */
constexpr float diffThreshold = 0.001F;

/**
 * @brief サーボのPWM周期
 */
constexpr int pwmPeriodMs = 20;

/**
 * @brief DCモータ用FETへのPWM出力(duty比)
 */
constexpr float fetDuty = 0.5F;

/**
 * @brief モータへの出力を計算するクラス
 */
class Schneider {
public:
    Schneider();
    Schneider(const Schneider&) = delete;
    Schneider(Schneider&&) = default;
    auto operator=(const Schneider&) -> Schneider& = delete;
    auto operator=(Schneider&&) -> Schneider& = default;
    ~Schneider();
    void debug();

    /**
     * @brief 1ステップ毎にモータへの入力値を計算する関数
     */
    void one_step();
    void init();

    /**
     * @brief 機体を停止させる関数
     */
    void flip_shneider();

    /**
     * @brief ledを点滅させる関数
     *
     * @param num 点滅させるledの番号
     */
    void led(int num);

private:
    /**
     * @brief 機体の姿勢
     */
    float phi;

    /**
     * @brief ボタンが押されたときに機体を停止させる関数(割り込み処理)
     */
    void ticker_flip();

    /**
     * @brief ヤコビ行列
     */
    std::array<std::array<float, 3>, 4> t_jacobianmatrix;

    /**
     * @brief ジョイスティックからの入力(目標値)
     */
    std::array<float, 3> x_d;

    /**
     * @brief 入力値
     */
    std::array<float, 4> q;

    /**
     * @brief qに対しての出力
     */
    std::array<float, 3> x;

    /**
     * @brief ヤコビ行列の計算を行う関数\nヤコビ行列は、入力からモータの出力を計算するための行列
     */
    void cal_tjacob();

    /**
     * @brief 状態方程式の計算を行う関数
     */
    void state_equation();

    /**
     * @brief
     * モータへの出力を計算する関数\nモータへの出力は、勾配を使って目的関数を最小化するように計算する
     */
    void cal_q();

    /**
     * @brief モータへの信号値に変換する関数
     */
    void set_q(const std::array<float, 3>& gyro);

    /**
     * @brief ジョイコンの値を読み取って、x_dに格納する
     *
     * @param joy_x ジョイコンのx軸の値
     * @param joy_y ジョイコンのy軸の値
     * @param rotate 回転の値
     */
    void read_joy(float joy_x, float joy_y, int rotate);

    /**
     * @brief つまみの値をから機体を回転させる関数
     */
    void rotate(const float& volume_value);

    /**
     * @brief ジャイロセンサの値を読み取る
     */
    auto read_gyro() -> std::array<float, 3>;

    AnalogIn adcIn1;  // ジョイスティック
    AnalogIn adcIn2;  // ジョイスティック
    AnalogIn volume;
    MPU6050 mpu;  // 慣性計測ユニット(imu)

    PwmOut servo_1;  // servo
    PwmOut servo_2;  // servo
    PwmOut fet_1;    // DC
    PwmOut fet_2;    // DC

    DigitalOut led1;
    DigitalOut led2;
    DigitalOut led3;

    BufferedSerial pc;
};
}  // namespace omniboat

#endif
