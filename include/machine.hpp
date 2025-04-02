#ifndef OMNIBOAT_R_INCLUDED
#define OMNIBOAT_R_INCLUDED

#include <array>

#include "mbed.h"

#include "device/input.hpp"
#include "device/output.hpp"
#include "packet/input.hpp"
#include "packet/output.hpp"
#include "service/service.hpp"

namespace omniboat {

/**
 * @brief モータへの出力を計算するクラス
 */
class Machine {
public:
    Machine(const Machine&) = delete;
    Machine(Machine&&) = default;
    auto operator=(const Machine&) -> Machine& = delete;
    auto operator=(Machine&&) -> Machine& = default;
    ~Machine();
    void debug();

    Machine(std::unique_ptr<device::InputModules>&& inputmodules,std::unique_ptr<device::OutputModules>&& outputmodules);

    /**
     * @brief 1ステップ毎にモータへの入力値を計算する関数
     */
    void one_step();
    void init();

    // TODO:
    // これ実装されてないのでコメントアウト
    // commit `f50fb9b` 参照
    // /**
    //  * @brief 機体を停止させる関数
    //  */
    // void flip_shneider();

    /**
     * @brief Machineをbuildするクラス
     * @param _joy_x_pin ジョイスティックのピン(x)
     * @param _joy_y_pin ジョイスティックのピン(y)
     * @param volume_pin ツマミのピン
     * @param _mpu_sda_pin MPU6050のピン (sda)
     * @param _mpu_scl_pin MPU6050のピン (scl)
     * @param servo_pins サーボのピン2つ
     * @param dc_motor_pins DCモーター (につながっているFET) のピン2つ
     */
    class Builder {
    private:
        PinName _joy_x_pin;
        PinName _joy_y_pin;
        PinName _volume_pin;
        PinName _mpu_sda_pin;
        PinName _mpu_scl_pin;
        std::pair<PinName,PinName> _servo_pins;
        std::pair<PinName,PinName> _dc_motor_pins;

    public:
        auto joy_x_pin(const PinName& pin) -> Builder&;
        auto joy_y_pin(const PinName& pin) -> Builder&;
        auto volume_pin(const PinName& pin) -> Builder&;
        auto mpu_sda_pin(const PinName& pin) -> Builder&;
        auto mpu_scl_pin(const PinName& pin) -> Builder&;
        auto servo_pins(const std::pair<PinName,PinName>& pins) -> Builder&;
        auto dc_motor_pins(const std::pair<PinName,PinName>& pins) -> Builder&;
        auto build() -> Machine;
        
        Builder() = default;
        ~Builder() = default;
        Builder(const Builder&) = delete;
        auto operator=(const Builder&) -> Builder& = delete;
        Builder(Builder&&) = default;
        auto operator=(Builder&&) -> Builder& = default;
};

static auto builder() -> Builder;

private:
    // flip_shneider と同様
    // /**
    //  * @brief ボタンが押されたときに機体を停止させる関数(割り込み処理)
    //  */
    // void ticker_flip();

    std::unique_ptr<device::InputModules> input_modules;
    std::unique_ptr<device::OutputModules> output_modules;
    service::Service service;
};

}  // namespace omniboat

#endif
