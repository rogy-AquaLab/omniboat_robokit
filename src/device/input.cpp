#include "device/input.hpp"

auto device::InputModules::read_joy() -> std::pair<float, float> {
    return {this->joy.first.read(), this->joy.second.read()};
}

auto device::InputModules::read_gyro() -> std::array<float, 3> {
    std::array<float, 3> gyro;
    this->mpu.getGyro(gyro.data());
    return gyro;
}

// NOLINTBEGIN(bugprone-easily-swappable-parameters)
device::InputModules::InputModules(
    const std::pair<PinName, PinName>& joy_pins, const PinName& volume_pin,
    const std::pair<PinName, PinName>& mpu_pins) :
    joy(joy_pins.first, joy_pins.second),
    volume(volume_pin),
    mpu(mpu_pins.first, mpu_pins.second) {}
// NOLINTEND(bugprone-easily-swappable-parameters)

auto device::InputModules::mpu_whoami() -> bool {
    return this->mpu.testConnection();
}

auto device::InputModules::read() -> packet::InputValues {
    const std::pair<float, float> joy = this->read_joy();
    const float volume = this->volume.read();
    const std::array<float, 3> gyro = this->read_gyro();
    return packet::InputValues(joy, volume, gyro);
}
