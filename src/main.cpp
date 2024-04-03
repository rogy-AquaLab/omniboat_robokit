#include "schneider_model.hpp"

int main() {
    omniboat::Schneider schneider;
    schneider.init();
    schneider.debug();
    while (true) {
        schneider.led(true);
        schneider.one_step();
        // wait(1);
    }
}
