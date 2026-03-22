#ifndef __MOTOR_H_
#define __MOTOR_H_


#include "AllHeader.h"


#define MAXpwm	1000	//Counter Compare Value 设置中
#define XF	1	//限幅开关
#define xf(n)	((n > MAXpwm) ? MAXpwm : n)	//限幅函数

#define STBY_1	DL_GPIO_setPins(PWM_STBY_PORT, PWM_STBY_PIN)
#define STBY_0	DL_GPIO_clearPins(PWM_STBY_PORT, PWM_STBY_PIN)

void Motor_Init(void);
void pwmA(int16_t pwm);
void pwmB(int16_t pwm);
void Motor_Stop(uint8_t mode);





/*
 *
 * A/B 左右电机有四种模式
 *
 * 0 反转
 * 1 正转
 * 2 刹车（轮有阻力）
 * 3 停止（轮无阻力）
 *
 */

static inline void Motor_A_0(void) {
	DL_GPIO_setPins(PWM_AIN1_PORT, PWM_AIN1_PIN);
	DL_GPIO_clearPins(PWM_AIN2_PORT, PWM_AIN2_PIN);
}

static inline void Motor_A_1(void) {
	DL_GPIO_setPins(PWM_AIN2_PORT, PWM_AIN2_PIN);
	DL_GPIO_clearPins(PWM_AIN1_PORT, PWM_AIN1_PIN);
}

static inline void Motor_A_2(void) {
	DL_GPIO_clearPins(PWM_AIN1_PORT, PWM_AIN1_PIN);
	DL_GPIO_clearPins(PWM_AIN2_PORT, PWM_AIN2_PIN);
}

static inline void Motor_A_3(void) {
	DL_GPIO_setPins(PWM_AIN1_PORT, PWM_AIN1_PIN);
	DL_GPIO_setPins(PWM_AIN2_PORT, PWM_AIN2_PIN);
}

static inline void Motor_B_0(void) {
	DL_GPIO_setPins(PWM_BIN1_PORT, PWM_BIN1_PIN);
	DL_GPIO_clearPins(PWM_BIN2_PORT, PWM_BIN2_PIN);
}

static inline void Motor_B_1(void) {
	DL_GPIO_setPins(PWM_BIN2_PORT, PWM_BIN2_PIN);
	DL_GPIO_clearPins(PWM_BIN1_PORT, PWM_BIN1_PIN);
}

static inline void Motor_B_2(void) {
	DL_GPIO_clearPins(PWM_BIN1_PORT, PWM_BIN1_PIN);
	DL_GPIO_clearPins(PWM_BIN2_PORT, PWM_BIN2_PIN);
}

static inline void Motor_B_3(void) {
	DL_GPIO_setPins(PWM_BIN1_PORT, PWM_BIN1_PIN);
	DL_GPIO_setPins(PWM_BIN2_PORT, PWM_BIN2_PIN);
}

#endif
