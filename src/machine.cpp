#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <thread>
#include <utility>

#include "rtos.h"

#include "machine.hpp"
#include "trace.hpp"
#include "utils.hpp"

namespace omniboat {

using trace::LedId;

/**
 * @brief z軸周りの慣性モーメント
 * @note もっと正確な値の方がいいかも
 */
constexpr float inertia_z = 1;
/**
 * @brief ステップ幅
 */
constexpr float step_width_a = 0.1;

Machine::Machine(std::unique_ptr<device::InputModules>&& inputmodules, std::unique_ptr<device::OutputModules>&& outputmodules) :
    input_modules(std::move(inputmodules)),output_modules(std::move(outputmodules)),service() {
    trace::toggle(LedId::First);
    trace::toggle(LedId::Second);
    trace::toggle(LedId::Third);
    printf("start up\n");
    this->output_modules->init();
}

Machine::~Machine() {
    trace::toggle(LedId::First);
}

void Machine::init() {
    this->service.init();
    const bool whoami = this->input_modules->mpu_whoami();
    if (whoami) {
        printf("WHOAMI succeeded\n");
    } else {
        printf("WHOAMI failed\n");
    }
}

inline auto map_joy(const std::pair<float, float>& joy) -> std::array<float, 3> {
    // ジョイスティックの中心値
    constexpr float joy_center = 0.5F;
    // [2]はrot
    return {(joy.first - joy_center) * 2, (joy.second - joy_center) * 2, 0};
}

void Machine::one_step() {
    const packet::InputValues input = this->input_modules->read();
    const packet::OutputValues output = this->service.call(input);
    this->output_modules->write(output);
    trace::toggle(LedId::Third);
}

void Machine::debug() {
    // printf("ave=%f\n",ave_[0]/ave_[1]);
    // printf("motor=%f,%f\n",q[0],q[1]);
    // printf("servo=%f,%f\n",q[2],q[3]);
    // printf("volume=%f\n",volume.read());
    // printf("gyro[2]=%f\n",gyro[2]);
    // printf("\n");
}

auto Machine::Builder::joy_x_pin(const PinName& pin) -> Builder& {
    this->_joy_x_pin = pin;
    return *this;
}

auto Machine::Builder::joy_y_pin(const PinName& pin) -> Builder& {
    this->_joy_y_pin = pin;
    return *this;
}

auto Machine::Builder::volume_pin(const PinName& pin) -> Builder& {
    this->_volume_pin = pin;
    return *this;
}

auto Machine::Builder::mpu_sda_pin(const PinName& pin) -> Builder& {
    this->_mpu_sda_pin = pin;
    return *this;
}

auto Machine::Builder::mpu_scl_pin(const PinName& pin) -> Builder& {
    this->_mpu_scl_pin = pin;
    return *this;
}

auto Machine::Builder::servo_pins(const std::pair<PinName,PinName>& pins) -> Builder& {
    this->_servo_pins = pins;
    return *this;
}

auto Machine::Builder::dc_motor_pins(const std::pair<PinName,PinName>& pins) -> Builder& {
    this->_dc_motor_pins = pins;
    return *this;
}

auto Machine::Builder::build() -> Machine{
    auto mpu = std::make_unique<MPU6050>(this->_mpu_sda_pin,this->_mpu_scl_pin);
    auto inputmodules = std::make_unique<device::InputModules>(std::make_pair(this->_joy_x_pin,this->_joy_y_pin),
    this->_volume_pin,std::move(mpu));
    auto outputmodules = std::make_unique<device::OutputModules>(this-> _servo_pins,this->_dc_motor_pins);
    return Machine(std::move(inputmodules),std::move(outputmodules));
}

auto Machine::builder() -> Builder{
    return Builder();
}

}  // namespace omniboat