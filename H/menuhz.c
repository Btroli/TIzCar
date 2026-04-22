#include "menuhz.h"


#define value_len 10
#define option_NUM 10
int value_num = 0;
int option = 0;
int count = 1;
int area = 0;
volatile int LoopMode = 0;
		//int LoopMode=0;



uint8_t name[10][10] = {
	"LKp",		//0
	"LKi",		//1
	"LKd",		//2
	"GAB",		//3
	"quan",		//4
	"BiZhang",		//5
	"name6",		//6
	"name7",		//7
	"name8",		//8
	"distance",		//9
};
uint8_t value[10][value_len] = {
	{0, 3, 8, 0, 0, 0, 0, 0, 0, 3},		//0
	{0, 0, 0, 0, 6, 0, 0, 0, 0, 3},		//1
	{0, 6, 6, 0, 0, 0, 0, 0, 0, 3},		//2
	{3, 0, 0, 0, 0, 0, 0, 0, 0, 2},		//3
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 3},		//4
	{0, 1, 0, 0, 0, 0, 0, 0, 0, 2},		//5
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 5},		//6
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 5},		//7
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 5},		//8
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 5},		//9
};
void loop_screen0(void) {		//主菜单，模式选择
	OLED_ShowString(6, 12 + 18 * 0, name[option - area + 0], 12, 1);
	OLED_ShowString(6, 12 + 18 * 1, name[option - area + 1], 12, 1);
	OLED_ShowString(6, 12 + 18 * 2, name[option - area + 2], 12, 1);

	OLED_DrawBoxLine(1, 9, 126, 25, 1);
	OLED_DrawBoxLine(1, 27, 126, 43, 1);
	OLED_DrawBoxLine(1, 45, 126, 61, 1);

	if (count)
		FuXuan(area);
	else
		FuXuan(area + 3);

	OLED_Refresh();
	OLED_ClearRF();

	count = 1 - count;
	if (KEY_Scan3()) {
		LoopMode = Loop_SCREEN1;	//进入下一个界面
		OLED_Clear();
		return;
	}
	if (KEY_Scan4()) {
		LoopMode = Loop_CHOICE;		//#define Loop_CHOICE     2
		OLED_Clear();
		return;
	}
	if (KEY_Scan1()) {		//上
		if (option < option_NUM - 1)
			option++;
		if (area < 2)
			area++;
	}
	if (KEY_Scan2()) {		//下
		if (option > 0)
			option--;
		if (area > 0)
			area--;
	}


	delay_ms(200);
}
void loop_screen1(void) {
	if (KEY_Scan1()) {
		value_num++;
		if (value_num == value_len)
			value_num = 0;
	}
	if (KEY_Scan2()) {
		if (value_num == value[option][value_len - 1]) {
			value[option][value_len - 1] += (value[option][value_len - 1] == 9) ? -9 : 1;
			value_num += (value_num == value_len - 1) ? 1 - value_len : 1;
		} else if (value_num < value[option][value_len - 1])
			value[option][value_num] += (value[option][value_num] == 9) ? -9 : 1;
		else
			value[option][value_num - 1] += (value[option][value_num - 1] == 9) ? -9 : 1;
	}
	for (uint8_t n = 0; n < value_len; n++) {
		if (n == value[option][value_len - 1])
			OLED_ShowChar(6 + n * 6, 30, '.', 12, (n == value_num && count) ? 0 : 1);
		else if (n < value[option][value_len - 1])
			OLED_ShowChar(6 + n * 6, 30, value[option][n] + '0', 12, (n == value_num && count) ? 0 : 1);
		else
			OLED_ShowChar(6 + n * 6, 30, value[option][n - 1] + '0', 12, (n == value_num && count) ? 0 : 1);
	}
	OLED_ShowString(6, 12, name[option], 12, 1);

	OLED_Refresh();
	OLED_Clear();

	if (KEY_Scan3()) {
		LoopMode = Loop_CHOICE;
		value_num = 0;
		OLED_Clear();
		return;
	}
	if (KEY_Scan4()) {
		LoopMode = Loop_SCREEN0;
		value_num = 0;
		OLED_Clear();
		return;
	}
	count = 1 - count;
	delay_ms(200);
}




void FuXuan(uint8_t n) {
	if (n < 3) {
		OLED_DrawBoxXuLine(0, 8 + 18 * n, 127, 26 + 18 * n, 3, 1);
		OLED_DrawBoxXuLine(2, 10 + 18 * n, 125, 24 + 18 * n, 3, 1);
	} else {
		n -= 3;
		OLED_DrawBoxLine(0, 8 + 18 * n, 127, 26 + 18 * n, 1);
		OLED_DrawBoxLine(2, 10 + 18 * n, 125, 24 + 18 * n, 1);
		OLED_DrawBoxXuLine(0, 8 + 18 * n, 127, 26 + 18 * n, 3, 0);
		OLED_DrawBoxXuLine(2, 10 + 18 * n, 125, 24 + 18 * n, 3, 0);
	}
}




uint8_t	KEY_Scan1() {
	if (DL_GPIO_readPins(KEY_PORT, KEY_KEY1_PIN) == key_pressure) {			//检测按键是否被按下
		delay_ms(10);			//延时消抖
		if (DL_GPIO_readPins(KEY_PORT, KEY_KEY1_PIN) == key_pressure) {		//再次检测是否为低电平
			while (DL_GPIO_readPins (KEY_PORT, KEY_KEY1_PIN) == key_pressure);		//等待按键放开
			return 1;		//返回按键按下标志
		}
	}
	return 0;
}
uint8_t	KEY_Scan2() {
	if (DL_GPIO_readPins(KEY_PORT, KEY_KEY2_PIN) == key_pressure) {			//检测按键是否被按下
		delay_ms(10);		//延时消抖
		if (DL_GPIO_readPins(KEY_PORT, KEY_KEY2_PIN) == key_pressure) {		//再次检测是否为低电平
			while (DL_GPIO_readPins (KEY_PORT, KEY_KEY2_PIN) == key_pressure);		//等待按键放开
			return 1;		//返回按键按下标志
		}
	}
	return 0;
}
uint8_t	KEY_Scan3() {
	if (DL_GPIO_readPins(KEY_PORT, KEY_KEY3_PIN) == key_pressure) {			//检测按键是否被按下
		delay_ms(10);		//延时消抖
		if (DL_GPIO_readPins(KEY_PORT, KEY_KEY3_PIN) == key_pressure) {		//再次检测是否为低电平
			while (DL_GPIO_readPins (KEY_PORT, KEY_KEY3_PIN) == key_pressure);		//等待按键放开
			return 1;
		}
	}
	return 0;
}
uint8_t	KEY_Scan4() {
	if (DL_GPIO_readPins(KEY_PORT, KEY_KEY4_PIN) == key_pressure) {			//检测按键是否被按下
		delay_ms(10);		//延时消抖
		if (DL_GPIO_readPins(KEY_PORT, KEY_KEY4_PIN) == key_pressure) {		//再次检测是否为低电平
			while (DL_GPIO_readPins (KEY_PORT, KEY_KEY4_PIN) == key_pressure);		//等待按键放开
			return 1;		//返回按键按下标志
		}
	}
	return 0;
}


