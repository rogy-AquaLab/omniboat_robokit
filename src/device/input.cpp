#include <memory>
#include <utility>

#include "device/input.hpp"

auto device::InputModules::read_joy() -> std::pair<float, float> {
    return {this->joy.first.read(), this->joy.second.read()};
}

auto device::InputModules::read_gyro() -> std::array<float, 3> {
    std::array<float, 3> gyro;
    this->mpu->getGyro(gyro.data());
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
    const mbed::AnalogIn joy_x(this->_joy_x_pin);
    const mbed::AnalogIn joy_y(this->_joy_y_pin);
    const mbed::AnalogIn volume(this->_volume_pin);
    unique_ptr<MPU6050> mpu = std::make_unique<MPU6050>(this->_mpu_sda_pin,this->_mpu_scl_pin);
    return InputModules{std::make_pair(joy_x,joy_y),volume,std::move(mpu)};
}

auto device::InputModules::mpu_whoami() -> bool {
    return this->mpu->testConnection();
}

auto device::InputModules::builder() -> Builder {
    return Builder();
}

auto device::InputModules::read() -> packet::InputValues {
    const std::pair<float, float> joy = this->read_joy();
    const float volume = this->volume.read();
    const std::array<float, 3> gyro = this->read_gyro();
    return packet::InputValues(joy, volume, gyro);
}
