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

Machine::Machine() :
    input_modules(device::InputModules::Builder()
        .joy_x_pin(A4)
        .joy_y_pin(A5)
        .volume_pin(A6)
        .mpu_sda_pin(D4)
        .mpu_scl_pin(D5)
        .build()),
    output_modules({PB_4, PA_11}, {PA_9, PA_10}), service() {
    trace::toggle(LedId::First);
    trace::toggle(LedId::Second);
    trace::toggle(LedId::Third);
    printf("start up\n");
    this->output_modules.init();
}

Machine::~Machine() {
    trace::toggle(LedId::First);
}

void Machine::init() {
    this->service.init();
    const bool whoami = this->input_modules.mpu_whoami();
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
    const packet::InputValues input = this->input_modules.read();
    const packet::OutputValues output = this->service.call(input);
    this->output_modules.write(output);
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

}  // namespace omniboat
