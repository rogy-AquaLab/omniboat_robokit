#include "schneider_model.hpp"

int main() {
    omniboat::Schneider schneider;
    schneider.init();
    schneider.debug();
    while (true) {
        schneider.led(1);
        schneider.one_step();
        // wait(1);
    }
    return 0;
}
