#include "machine.hpp"
#include "trace.hpp"

auto main() -> int {
    trace::init();
    omniboat::Machine machine(device::InputModules::builder()
    .joy_x_pin(A4)
    .joy_y_pin(A5)
    .volume_pin(A6)
    .mpu_sda_pin(D4)
    .mpu_scl_pin(D5)
    .build(),
    device::OutputModules::builder()
    .servo_pins({PB_4, PA_11})
    .dc_motor_pins({PA_9, PA_10})
    .build());
    machine.init();
    machine.debug();
    while (true) {
        trace::toggle(trace::LedId::First);
        machine.one_step();
        // wait(1);
    }
}
