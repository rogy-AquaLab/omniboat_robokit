#ifndef OMNIBOAT_R_INCLUDED
#define OMNIBOAT_R_INCLUDED

#include <array>

#include "mbed.h"

#include "MPU6050.h"
#include "device/input.hpp"
#include "packet/input.hpp"
#include "packet/output.hpp"

namespace omniboat {

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

private:
    /**
     * @brief ボタンが押されたときに機体を停止させる関数(割り込み処理)
     */
    void ticker_flip();

    /**
     * @brief 入力値
     */
    std::array<float, 4> inputs;

    /**
     * @brief inputsに対しての出力
     */
    std::array<float, 3> outputs;

    /**
     * @brief ヤコビ行列の計算を行う関数\nヤコビ行列は、入力からモータの出力を計算するための行列
     */
    std::array<std::array<float, 3>, 4> cal_tjacob() const;

    /**
     * @brief 最後にアクチュエーターに反映させた値
     */
    packet::OutputValues last_output;

    /**
     * @brief 状態方程式の計算を行う関数
     */
    void state_equation();

    /**
     * @brief
     * モータへの出力を計算する関数\nモータへの出力は、勾配を使って目的関数を最小化するように計算する
     */
    auto cal_q(const std::array<float, 3>& joy) -> void;

    /**
     * @brief モータへの信号値に変換する関数
     */
    auto set_q(const std::array<float, 3>& gyro) -> packet::OutputValues;

    /**
     * @brief つまみの値をから機体を回転させる関数
     */
    auto rotate(const float& volume_value) const -> packet::OutputValues;

    /**
     * @brief FETへの出力(=DCモーター)を止める関数
     * @return packet::OutputValues write_outputに渡す値
     */
    auto stop_fet() const -> packet::OutputValues;

    auto write_output(const packet::OutputValues& output) -> void;

    device::InputModules input_modules;

    PwmOut servo_1;  // servo
    PwmOut servo_2;  // servo
    PwmOut fet_1;    // DC
    PwmOut fet_2;    // DC
};
}  // namespace omniboat

#endif
