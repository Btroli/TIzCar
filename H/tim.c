#include "tim.h"

volatile uint16_t Stime;
volatile uint8_t LSread;

void TIMER_10ms_Init(void) {
	NVIC_ClearPendingIRQ(TIMER_10ms_INST_INT_IRQN);
	NVIC_EnableIRQ(TIMER_10ms_INST_INT_IRQN);
	DL_TimerG_startCounter(TIMER_10ms_INST);
}

void TIMER_20ms_Init(void) {
	NVIC_ClearPendingIRQ(TIMER_20ms_INST_INT_IRQN);
	NVIC_EnableIRQ(TIMER_20ms_INST_INT_IRQN);
	DL_TimerG_startCounter(TIMER_20ms_INST);
}

void PIDloop_Init(void) {
	NVIC_ClearPendingIRQ(PIDloop_INST_INT_IRQN);
	NVIC_EnableIRQ(PIDloop_INST_INT_IRQN);
	DL_TimerG_startCounter(PIDloop_INST);
}

void TIM_init(void) {
	TIMER_10ms_Init();
	TIMER_20ms_Init();
	PIDloop_Init();
}

/*******************************************************/

void TIMER_10ms_INST_IRQHandler(void) {
	if ( DL_TimerG_getPendingInterrupt(TIMER_10ms_INST) == DL_TIMER_IIDX_ZERO )
		Stime++;
}

void TIMER_20ms_INST_IRQHandler(void) {
	if ( DL_TimerG_getPendingInterrupt(TIMER_20ms_INST) == DL_TIMER_IIDX_ZERO ) {
		encoder_update();
		LS_update;
	}
}

void PIDloop_INST_IRQHandler(void) {
	if ( DL_TimerG_getPendingInterrupt(PIDloop_INST) == DL_TIMER_IIDX_ZERO ) {
		loop();
	}
}

