#ifndef _ULTRASOUND_H
#define _ULTRASOUND_H

#include "AllHeader.h"

#define TRIG_HIGH	DL_GPIO_setPins(ULTRASOUND_PORT, ULTRASOUND_TRIG_PIN)
#define TRIG_LOW	DL_GPIO_clearPins(ULTRASOUND_PORT, ULTRASOUND_TRIG_PIN)
#define TRIG_TOGGLE	DL_GPIO_togglePins(ULTRASOUND_PORT, ULTRASOUND_TRIG_PIN)

extern volatile int8_t  capture_done;
extern volatile uint16_t  echotime;

#define CSB (capture_done = 3)

/*
 * capture_done = 3->0 （中断，特殊）trig态，忽略
 * capture_done = 1 完成态，测量结束。
 * capture_done = 0 等待echo态
 * capture_done = -1 计时echo态
 */

float ultrasound_distance(void);

#endif

/*
   最原始状态

while (1) {
	TRIG_HIGH;
	delay_us(15);
	TRIG_LOW;

	//重置状态
	capture_done = 0;

	//等待测量完成
	while (capture_done == 1);
}
*/
