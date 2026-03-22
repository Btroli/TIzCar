#include "motor.h"

void Motor_Init(void) {
	STBY_1;
	DL_TimerA_startCounter(motorPWM_INST);
}

void pwmA(int16_t pwm) {
	if (pwm < 0) {
		Motor_A_0();
		pwm = -pwm;
	} else
		Motor_A_1();
#if XF
	pwm = xf(pwm);
#endif
	DL_TimerA_setCaptureCompareValue(motorPWM_INST, pwm, GPIO_motorPWM_C0_IDX);
}

void pwmB(int16_t pwm) {
	if (pwm < 0) {
		Motor_B_0();
		pwm = -pwm;
	} else
		Motor_B_1();
#if XF
	pwm = xf(pwm);
#endif
	DL_TimerA_setCaptureCompareValue(motorPWM_INST, pwm, GPIO_motorPWM_C1_IDX);
}

void Motor_Stop(uint8_t mode) {
	if (mode) {
		Motor_A_2();
		Motor_B_2();
		DL_TimerA_setCaptureCompareValue(motorPWM_INST, MAXpwm, GPIO_motorPWM_C0_IDX);
		DL_TimerA_setCaptureCompareValue(motorPWM_INST, MAXpwm, GPIO_motorPWM_C1_IDX);
	} else {
		Motor_A_3();
		Motor_B_3();
		DL_TimerA_setCaptureCompareValue(motorPWM_INST, 0, GPIO_motorPWM_C0_IDX);
		DL_TimerA_setCaptureCompareValue(motorPWM_INST, 0, GPIO_motorPWM_C1_IDX);
	}
}

