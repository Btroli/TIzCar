#ifndef _MENUHZ_H_
#define _MENUHZ_H_

#include "AllHeader.h"


#define key_pressure     0    // 按键按下
#define key_release      1    // 按键松开
#define Loop_SCREEN0    0
#define Loop_SCREEN1    1
#define Loop_CHOICE     2

extern volatile int LoopMode;
extern uint8_t value[10][10];

void cal_valueP(uint8_t *p, float *v);


void loop_screen0(void);
void loop_screen1(void);

uint8_t KEY_Scan1(void);
uint8_t KEY_Scan2(void);
uint8_t KEY_Scan3(void);
uint8_t KEY_Scan4(void);

void FuXuan(uint8_t n);

#endif
