#ifndef _TIM_H_
#define _TIM_H_

#include "AllHeader.h"

#define TIM_10		1
#define TIM_20		0
#define TIM_OLED	1
#define TIM_LOOP	1
#define TIM_TRIG	0

extern volatile uint16_t Stime;

extern void loop(void);
extern void OLED_loop(void);

void TIM_init(void);

#endif
