#ifndef __ALLHEADER_H_
#define __ALLHEADER_H_

#define u8  uint8_t
#define u16 uint16_t
#define u32 uint32_t

#define ReadKEY1 (DL_GPIO_readPins(KEY_PORT, KEY_KEY1_PIN)&KEY_KEY1_PIN)

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ti_msp_dl_config.h"

#include "delay.h"
#include "oled.h"
#include "LS.h"
#include "motor.h"

#endif

