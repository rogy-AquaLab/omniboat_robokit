#include "machine.hpp"
#include "trace.hpp"

auto main() -> int {
    trace::init();
    omniboat::Machine machine;
    machine.init();
    machine.debug();
    while (true) {
        trace::toggle(trace::LedId::First);
        machine.one_step();
        // wait(1);
    }
}
