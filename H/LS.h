#ifndef __LS_H
#define	__LS_H

#include "AllHeader.h"

extern volatile uint8_t LSread;

#define LS_ONE(pinNum)  ((DL_GPIO_readPins(LS_PORT, (1U << (pinNum))) >> (pinNum)) & 1U)
#define LS_READ         ((uint8_t)(DL_GPIO_readPins(LS_PORT, 0xFFU) & 0xFFU))

#if LINE
#define LS_update do { LSread = LS_READ; } while(0)
#else
#define LS_update do { LSread = ~LS_READ; } while(0)
#endif

/*
    LS: lightsensor 光敏传感器
    pin -> Pin0~Pin7
*/

#endif
