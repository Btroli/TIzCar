#ifndef __LS_H
#define	__LS_H

#include "AllHeader.h"
#include <ti/driverlib/dl_gpio.h>

// #define LS_ONE(pinNum)	((DL_GPIO_readPort(LS_PORT) >> (pinNum)) & 1)
// #define LS_READ		((uint8_t)DL_GPIO_readPort(LS_PORT))
// 编译问题，无法使用。

#define LS_ONE(pinNum)  (((DL_GPIO_readPins(LS_PORT, (1U << (pinNum))) >> (pinNum)) & 1U)
#define LS_READ         ((uint8_t)(DL_GPIO_readPins(LS_PORT, 0xFFU) & 0xFFU))

/*
    LS: lightsensor 光敏传感器
    pin -> Pin0~Pin7
*/

#endif
