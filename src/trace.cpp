#include <DigitalOut.h>
#include "trace.hpp"

#ifndef TRACE_LED1_PIN
#define TRACE_LED1_PIN PA_1
#endif // TRACE_LED1_PIN

#ifndef TRACE_LED2_PIN
#define TRACE_LED2_PIN PA_3
#endif // TRACE_LED2_PIN

#ifndef TRACE_LED3_PIN
#define TRACE_LED3_PIN PA_4
#endif // TRACE_LED3_PIN

namespace trace {
    mbed::DigitalOut led_first(TRACE_LED1_PIN);
    mbed::DigitalOut led_second(TRACE_LED2_PIN);
    mbed::DigitalOut led_third(TRACE_LED3_PIN);

    void init() {
        led_first.write(0);
        led_second.write(0);
        led_third.write(0);
    }

    void toggle(LedId id) {
        switch (id) {
        case LedId::First:
            led_first.write(1 - led_first.read());
            break;
        case LedId::Second:
            led_second.write(1 - led_second.read());
            break;
        case LedId::Third:
            led_third.write(1 - led_third.read());
            break;
        }
    }

    void on(LedId id) {
        switch (id) {
        case LedId::First:
            led_first.write(1);
            break;
        case LedId::Second:
            led_second.write(1);
            break;
        case LedId::Third:
            led_third.write(1);
            break;
        }
    }

    void off(LedId id) {
        switch (id) {
        case LedId::First:
            led_first.write(0);
            break;
        case LedId::Second:
            led_second.write(0);
            break;
        case LedId::Third:
            led_third.write(0);
            break;
        }
    }
}
