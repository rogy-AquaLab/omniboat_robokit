#ifndef OMNIBOAT_ROBOKIT_SERVICE_HPP
#define OMNIBOAT_ROBOKIT_SERVICE_HPP

#include "packet/input.hpp"
#include "packet/output.hpp"

namespace service {

class Service {
public:
    Service() = default;
    ~Service() = default;
    Service(const Service&) = default;
    auto operator=(const Service&) -> Service& = default;
    Service(Service&&) = default;
    auto operator=(Service&&) -> Service& = default;

    void init();

    auto call(const packet::InputValues& input_values) -> packet::OutputValues;

    // NOTE
    // 使われてないのでコメントアウト
    // 元の実装は f50fb9b の src/schneider_model.cpp:88 を参照
    // /**
    //  * @brief 機体を停止させる関数
    //  */
    // auto flip_shneider() -> void;

    // TODO: auto debug() -> void;

private:
    /// 入力値
    std::array<float, 4> inputs = {0, 0, 0};

    /// inputsに対しての出力
    std::array<float, 3> outputs = {0, 0, 0};

    /// 実際に出力された最後の値 outputsとは違う
    packet::OutputValues last_output{};

    /**
     * @brief ヤコビ行列の計算を行う関数
     *        ヤコビ行列は、入力からモータの出力を計算するための行列
     * @return std::array<std::array<float, 3>, 4> 計算したヤコビ行列
     */
    auto cal_tjacob() const -> std::array<std::array<float, 3>, 4>;

    /// 状態方程式の計算を行う関数
    auto state_equation() -> void;

    /// モータへの出力を計算する関数
    /// モータへの出力は、勾配を使って目的関数を最小化するように計算する
    auto cal_q(const std::array<float, 3>& joy) -> void;

    /**
     * @brief 機体を旋回(=平行移動+回転) させるように出力する関数
     * @param gyro MPUの入力 `packet::IntputValues::gyro`
     * @return packet::OutputValues アクチュエーターに出力する値 ここで`last_output`は更新しない
     */
    auto set_q(const std::array<float, 3>& gyro) -> packet::OutputValues;

    /**
     * @brief ツマミの値から機体を回転させるように出力する関数
     * @param volume ツマミの値
     * @return packet::OutputValues アクチュエーターに出力する値 ここで`last_output`は更新しない
     */
    auto rotate(const float& volume) const -> packet::OutputValues;

    /**
     * @brief DCモーター(につながっているFET)を止めるように出力する関数
     * @return packet::OutputValues アクチュエーターに出力する値 ここで`last_output`は更新しない
     */
    auto stop_dc_motor() const -> packet::OutputValues;
};

}  // namespace service

#endif  // OMNIBOAT_ROBOKIT_SERVICE_HPP
