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
    std::unique_ptr<MPU6050> mpu;

    /// ジョイスティックの値を読む
    auto read_joy() -> std::pair<float, float>;

    /// IMUの値を読む
    auto read_gyro() -> std::array<float, 3>;

    // NOLINTBEGIN(bugprone-easily-swappable-parameters)
    InputModules(
        std::pair<mbed::AnalogIn, mbed::AnalogIn> joy, mbed::AnalogIn volume,
        std::unique_ptr<MPU6050>&& mpu) :
        joy(joy), volume(volume), mpu(std::move(mpu)) {}
    // NOLINTEND(bugprone-easily-swappable-parameters)

public:
    /**
     * @brief InputModulesをbuildするクラス
     * @param _joy_x_pin ジョイスティックのピン(x)
     * @param _joy_y_pin ジョイスティックのピン(y)
     * @param volume_pin ツマミのピン
     * @param _mpu_sda_pin MPU6050のピン (sda)
     * @param _mpu_scl_pin MPU6050のピン (scl)
     */
    class Builder {
    private:
        PinName _joy_x_pin;
        PinName _joy_y_pin;
        PinName _volume_pin;
        PinName _mpu_sda_pin;
        PinName _mpu_scl_pin;

    public:
        auto joy_x_pin(const PinName& pin) -> Builder&;
        auto joy_y_pin(const PinName& pin) -> Builder&;
        auto volume_pin(const PinName& pin) -> Builder&;
        auto mpu_sda_pin(const PinName& pin) -> Builder&;
        auto mpu_scl_pin(const PinName& pin) -> Builder&;
        auto build() -> InputModules;

        Builder() = default;
        ~Builder() = default;
        Builder(const Builder&) = delete;
        auto operator=(const Builder&) -> Builder& = delete;
        Builder(Builder&&) = default;
        auto operator=(Builder&&) -> Builder& = default;
    };

    static auto builder() -> Builder;

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
