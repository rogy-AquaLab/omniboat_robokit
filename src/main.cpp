#include "machine.hpp"
#include "trace.hpp"

auto main() -> int {
    trace::init();
    omniboat::Machine Machine;
    Machine.init();
    Machine.debug();
    while (true) {
        trace::toggle(trace::LedId::First);
        Machine.one_step();
        // wait(1);
    }
}
