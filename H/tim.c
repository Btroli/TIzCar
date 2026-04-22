#include "tim.h"

volatile uint16_t Stime;
volatile uint8_t LSread;

#if TIM_10
void TIMER_10ms_Init(void) {
	NVIC_ClearPendingIRQ(TIMER_10ms_INST_INT_IRQN);
	NVIC_EnableIRQ(TIMER_10ms_INST_INT_IRQN);
	DL_TimerG_startCounter(TIMER_10ms_INST);
}
#endif

#if TIM_20
void TIMER_20ms_Init(void) {
	NVIC_ClearPendingIRQ(TIMER_20ms_INST_INT_IRQN);
	NVIC_EnableIRQ(TIMER_20ms_INST_INT_IRQN);
	DL_TimerG_startCounter(TIMER_20ms_INST);
}
#endif

#if TIM_OLED
void TIMER_OLED_Init(void) {
	NVIC_ClearPendingIRQ(TIMER_OLED_INST_INT_IRQN);
	NVIC_EnableIRQ(TIMER_OLED_INST_INT_IRQN);
	DL_TimerG_startCounter(TIMER_OLED_INST);
}
#endif

#if TIM_LOOP
void PIDloop_Init(void) {
	NVIC_ClearPendingIRQ(PIDloop_INST_INT_IRQN);
	NVIC_EnableIRQ(PIDloop_INST_INT_IRQN);
	DL_TimerG_startCounter(PIDloop_INST);
}
#endif

#if TIM_TRIG
void TIMER_TRIG_Init(void) {
	NVIC_ClearPendingIRQ(TIMER_TRIG_INST_INT_IRQN);
	NVIC_EnableIRQ(TIMER_TRIG_INST_INT_IRQN);
	DL_TimerA_startCounter(TIMER_TRIG_INST);
}
#endif

void TIM_init(void) {
#if TIM_10
	TIMER_10ms_Init();
#endif
#if TIM_20
	TIMER_20ms_Init();
#endif
#if TIM_OLED
	TIMER_OLED_Init();
#endif
#if TIM_LOOP
	PIDloop_Init();
#endif
#if TIM_TRIG
	TIMER_TRIG_Init();
#endif
}

/*******************************************************/

#if TIM_10
void TIMER_10ms_INST_IRQHandler(void) {
	if ( DL_TimerG_getPendingInterrupt(TIMER_10ms_INST) == DL_TIMER_IIDX_ZERO )
		Stime++;
}
#endif

#if TIM_20
void TIMER_20ms_INST_IRQHandler(void) {
	if ( DL_TimerG_getPendingInterrupt(TIMER_20ms_INST) == DL_TIMER_IIDX_ZERO ) {
		encoder_update();
		LS_update;                   //八路灰度传感器检测
	}
}
#endif

#if TIM_OLED
void TIMER_OLED_INST_IRQHandler(void) {
	if ( DL_TimerG_getPendingInterrupt(TIMER_OLED_INST) == DL_TIMER_IIDX_ZERO )

			OLED_loop();

}
#endif

#if TIM_LOOP
void PIDloop_INST_IRQHandler(void) {
	if ( DL_TimerG_getPendingInterrupt(PIDloop_INST) == DL_TIMER_IIDX_ZERO ) {
		if(LoopMode==Loop_CHOICE)loop();
		
		else{Motor_Stop(0);}
	}
}
#endif

#if TIM_TRIG
void TIMER_TRIG_INST_IRQHandler(void) {
	if ( DL_TimerA_getPendingInterrupt(TIMER_TRIG_INST) == DL_TIMER_IIDX_ZERO )
		if (capture_done > 1) {
			if (capture_done-- == 2) {
				capture_done = 0;
				TRIG_LOW;
				return;
			}
			TRIG_HIGH;
		}
}
#endif

