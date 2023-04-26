#include <chrono>
#include "mbed.h"
#include "rtos.h"

int main() {
	PwmOut servo(PB_4);
	servo.period_ms(20);
	servo.pulsewidth_us(1450);
	while (1) {}
	return 0;
}

