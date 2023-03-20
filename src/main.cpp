#include "schneider_model.h"

int main() {
    omniboat::Schneider schneider;
    schneider.init();
    schneider.debug();
    while (1) {
        schneider.led(1);
        schneider.one_step();
        // wait(1);
    }
}
