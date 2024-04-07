#include "schneider_model.hpp"

auto main() -> int {
    omniboat::Schneider schneider;
    schneider.init();
    schneider.debug();
    while (true) {
        schneider.led(1);
        schneider.one_step();
        // wait(1);
    }
}
