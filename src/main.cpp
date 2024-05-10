#include "schneider_model.hpp"
#include "trace.hpp"

auto main() -> int {
    trace::init();
    omniboat::Schneider schneider;
    schneider.init();
    schneider.debug();
    while (true) {
        trace::toggle(trace::LedId::First);
        schneider.one_step();
        // wait(1);
    }
}
