#ifndef _TIM_H_
#define _TIM_H_

#include "AllHeader.h"

extern volatile uint16_t Stime;

extern void loop(void);

void TIM_init(void);

#endif
