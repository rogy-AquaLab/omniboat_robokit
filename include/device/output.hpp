#ifndef OMNIBOAT_ROBOKIT_DEVICE_OUTPUT_HPP
#define OMNIBOAT_ROBOKIT_DEVICE_OUTPUT_HPP

#include <utility>

#include "PwmOut.h"

#include "packet/output.hpp"

namespace device {

/// アクチュエーター類
class OutputModules {
private:
    /// サーボ2つ
    std::pair<mbed::PwmOut, mbed::PwmOut> servo;

    /// DCモーター (につながっているFET) 2つ
    std::pair<mbed::PwmOut, mbed::PwmOut> dc_motor;

    /**
     * @brief サーボに出力する
     * @param values 出力する値 `OutputValues::servo` と同じ
     */
    auto write_servo(const std::pair<float, float>& values) -> void;

    /**
     * @brief DCモーターに出力する
     * @param values 出力する値 `OutputModules::dc_motor` と同じ
     */
    auto write_dc_motor(const std::pair<float, float>& values) -> void;

    OutputModules(
        std::pair<mbed::PwmOut,mbed::PwmOut> servo,
        std::pair<mbed::PwmOut,mbed::PwmOut> dc_motor) :
        servo(servo),dc_motor(dc_motor) {}

public:
    /**
     * @brief Construct a new Output Modules object
     * @param servo_pins サーボのピン2つ
     * @param dc_motor_pins DCモーター (につながっているFET) のピン2つ
     */
    class Builder {
    private:
        std::pair<PinName,PinName> _servo_pins;
        std::pair<PinName,PinName> _dc_motor_pins;

    public:
        auto servo_pins(const std::pair<PinName,PinName>& pins) -> Builder&;
        auto dc_motor_pins(const std::pair<PinName,PinName>& pins) -> Builder&;
        auto build() -> OutputModules;

        Builder() = default;
        ~Builder() = default;
        Builder(const Builder&) = delete;
        auto operator=(const Builder&) -> Builder& = delete;
        Builder(Builder&&) = default;
        auto operator=(Builder&&) -> Builder& = default;
    };

    static auto builder() -> Builder;

    OutputModules() = delete;
    ~OutputModules() = default;
    OutputModules(const OutputModules&) = delete;
    auto operator=(const OutputModules&) -> OutputModules& = delete;
    OutputModules(OutputModules&&) = default;
    auto operator=(OutputModules&&) -> OutputModules& = default;

    /**
     * @brief セットアップ処理
     * @details PWMの周期設定
     */
    auto init() -> void;

    /**
     * @brief アクチュエーターに出力する
     * @param output 出力する値
     */
    auto write(const packet::OutputValues& output) -> void;
};

}  // namespace device

#endif  // OMNIBOAT_ROBOKIT_DEVICE_OUTPUT_HPP
