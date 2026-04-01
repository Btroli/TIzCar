#ifndef __ALLHEADER_H_
#define __ALLHEADER_H_

#define u8  uint8_t
#define u16 uint16_t
#define u32 uint32_t

#define BLACK_LINE 1
#define WHITE_LINE 0
#define LINE BLACK_LINE

#define LEFT	0b11100000
#define MID 	0b00011000
#define RIGHT	0b00000111

#define ReadKEY1 (DL_GPIO_readPins(KEY_PORT, KEY_KEY1_PIN)&KEY_KEY1_PIN)

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ti_msp_dl_config.h"

#include "delay.h"
#include "oled.h"
#include "LS.h"
#include "motor.h"
#include "encoder.h"
#include "tim.h"
#include "ultrasound.h"

#endif

/*
 * GLOBAL 全局变量集合
 *
 * Stime
 * ecdA
 * ecdB
 * LSread
 *
 */

