#include "ultrasound.h"

volatile int8_t  capture_done = 1;
volatile uint16_t  echotime = 0xFFFF;

void Ultrasound_init(void) {
	NVIC_ClearPendingIRQ(CAPTURE_ULTRASOUND_ECHO_INST_INT_IRQN);
	NVIC_EnableIRQ(CAPTURE_ULTRASOUND_ECHO_INST_INT_IRQN);
}

void CAPTURE_ULTRASOUND_ECHO_INST_IRQHandler(void) {
	if (DL_Timer_getRawInterruptStatus(CAPTURE_ULTRASOUND_ECHO_INST, DL_TIMER_INTERRUPT_CC0_UP_EVENT)) {
		DL_Timer_clearInterruptStatus(CAPTURE_ULTRASOUND_ECHO_INST, DL_TIMER_INTERRUPT_CC0_UP_EVENT);

		if (capture_done == 0) {
			DL_Timer_setTimerCount(CAPTURE_ULTRASOUND_ECHO_INST, 0);
			capture_done = -1;
		} else if (capture_done == -1) {
			echotime = DL_Timer_getCaptureCompareValue(CAPTURE_ULTRASOUND_ECHO_INST, DL_TIMER_CC_0_INDEX);
			capture_done = 1;
		}
	}
}

float ultrasound_distance(void) {
	return echotime * 0.017;
}

