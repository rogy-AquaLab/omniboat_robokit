#include <utility>

#include "device/input.hpp"

auto device::InputModules::read_joy() -> std::pair<float, float> {
    return {this->joy.first.read(), this->joy.second.read()};
}

auto device::InputModules::read_gyro() -> std::array<float, 3> {
    std::array<float, 3> gyro;
    this->mpu.getGyro(gyro.data());
    return gyro;
}

auto device::InputModules::Builder::joy_x_pin(const PinName& pin) -> Builder& {
    this->_joy_x_pin = pin;
    return *this;
}

auto device::InputModules::Builder::joy_y_pin(const PinName& pin) -> Builder& {
    this->_joy_y_pin = pin;
    return *this;
}

auto device::InputModules::Builder::volume_pin(const PinName& pin) -> Builder& {
    this->_volume_pin = pin;
    return *this;
}

auto device::InputModules::Builder::mpu_sda_pin(const PinName& pin) -> Builder& {
    this->_mpu_sda_pin = pin;
    return *this;
}

auto device::InputModules::Builder::mpu_scl_pin(const PinName& pin) -> Builder& {
    this->_mpu_scl_pin = pin;
    return *this;
}

auto device::InputModules::Builder::build() -> InputModules {
    return InputModules(this);
}

auto device::InputModules::mpu_whoami() -> bool {
    return this->mpu.testConnection();
}

auto device::InputModules::builder() -> Builder {
    return Builder();
}

device::InputModules::InputModules(Builder* builder) :
    joy(builder->_joy_x_pin, builder->_joy_y_pin),
    volume(builder->_volume_pin),
    mpu(builder->_mpu_sda_pin, builder->_mpu_scl_pin) {}

auto device::InputModules::read() -> packet::InputValues {
    const std::pair<float, float> joy = this->read_joy();
    const float volume = this->volume.read();
    const std::array<float, 3> gyro = this->read_gyro();
    return packet::InputValues(joy, volume, gyro);
}
