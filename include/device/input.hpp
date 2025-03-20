#ifndef OMNIBOAT_ROBOKIT_DEVICE_INPUT_HPP
#define OMNIBOAT_ROBOKIT_DEVICE_INPUT_HPP

#include <array>
#include <utility>

#include "AnalogIn.h"

#include "MPU6050.h"
#include "packet/input.hpp"

namespace device {

/// センサー類まとめ
class InputModules {
private:
    /// ジョイスティックのピン２つ
    std::pair<mbed::AnalogIn, mbed::AnalogIn> joy;

    /// ツマミ
    mbed::AnalogIn volume;

    /// 慣性計測ユニット(imu)
    MPU6050 mpu;

    /// ジョイスティックの値を読む
    auto read_joy() -> std::pair<float, float>;

    /// IMUの値を読む
    auto read_gyro() -> std::array<float, 3>;

public:
    /**
     * @brief Construct a new Input Modules object
     * @param joy_pins ジョイスティックのピン2つ
     * @param volume_pin ツマミのピン
     * @param mpu_pins MPU6050のピン (sda, scl)
     */
    class Builder {
    private:
        PinName _joy_x_pin;
        PinName _joy_y_pin;
        PinName _volume_pin;
        PinName _mpu_sda_pin;
        PinName _mpu_scl_pin;

    public:
        auto joy_x_pin(const PinName& pin) -> Builder&; //joy_pins.first
        auto joy_y_pin(const PinName& pin) -> Builder&; //joy_pins.second
        auto volume_pin(const PinName& pin) -> Builder&;
        auto mpu_sda_pin(const PinName& pin) -> Builder&; //mpu_pins.first
        auto mpu_scl_pin(const PinName& pin) -> Builder&; //mpu_pins.second
        auto build() -> InputModules;
    };

public:
    static auto builder() -> Builder;

    InputModules(Builder* builder);

    InputModules() = delete;
    ~InputModules() = default;
    InputModules(const InputModules&) = delete;
    auto operator=(const InputModules&) -> InputModules& = delete;
    InputModules(InputModules&&) = default;
    auto operator=(InputModules&&) -> InputModules& = default;

    /**
     * @brief MPU6050のWHOAMI `MPU6050::testConnection` 参照
     * @return true success
     * @return false failure
     */
    auto mpu_whoami() -> bool;

    /**
     * @brief センサー類の値を読む
     * @return packet::InputValues 読んだ値まとめ
     */
    auto read() -> packet::InputValues;
};

}  // namespace device

#endif  // OMNIBOAT_ROBOKIT_DEVICE_INPUT_HPP
