#include "device/output.hpp"
#include "utils.hpp"

// サーボモータ出力値(pulse width)
// FIXME: もともと550, 2350だったので要検証
constexpr int minor_pulsewidth_us = 500;
constexpr int major_pulsewidth_us = 2400;

/// ラジアン → PWM pulsewidth_us
constexpr auto servo_value_map(float radian) -> int {
    return static_cast<int>(
        (major_pulsewidth_us - minor_pulsewidth_us) * (radian / utils::PI) + minor_pulsewidth_us);
}

auto device::OutputModules::write_servo(const std::pair<float, float>& values) -> void {
    this->servo.first.pulsewidth_us(servo_value_map(values.first));
    this->servo.second.pulsewidth_us(servo_value_map(values.second));
}

auto device::OutputModules::write_dc_motor(const std::pair<float, float>& values) -> void {
    this->dc_motor.first.write(values.first);
    this->dc_motor.second.write(values.second);
}

auto device::OutputModules::Builder::servo_pins(const std::pair<PinName,PinName>& pins) -> Builder& {
    this->_servo_pins = pins;
    return *this;
}

auto device::OutputModules::Builder::dc_motor_pins(const std::pair<PinName,PinName>& pins) -> Builder& {
    this->_dc_motor_pins = pins;
    return *this;
}

auto device::OutputModules::Builder::build() -> OutputModules {
    return OutputModules(_servo_pins,_dc_motor_pins);
}

auto device::OutputModules::builder() -> OutputModules::Builder {
    return OutputModules::Builder();
}

auto device::OutputModules::init() -> void {
    constexpr int pwm_period_ms = 20;

    this->servo.first.period_ms(pwm_period_ms);
    this->servo.second.period_ms(pwm_period_ms);
}

auto device::OutputModules::write(const packet::OutputValues& output) -> void {
    this->write_servo(output.servo);
    this->write_dc_motor(output.dc_motor);
}
