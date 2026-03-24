#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "AllHeader.h"

extern volatile int16_t ecdA, ecdB;

void encoder_init(void);
void encoder_update(void);

#endif
