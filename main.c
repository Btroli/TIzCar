#include "ti_msp_dl_config.h"

#include "AllHeader.h"

void pid0(void);
void pid1(void);
void OLED_loop(void);

void loop_pid1(void);

int main(void) {
	SYSCFG_DL_init();

	OLED_SET();
	Motor_init();
	encoder_init();
	TIM_init();

	while (1);
}


//loop
uint8_t Last_LSread;
int8_t GA = 0, GB = 0;

//pid0
int16_t SPDA = 0, SPDB = 0, PA = 0, PB = 0, pre_PA = 0, pre_PB = 0, sum_PA = 0, sum_PB = 0;

//pid1
static const int8_t jq[4] = {8, 12, 20, 15};
volatile float Kp = 4.7, Ki = 0, Kd = 17;
int8_t Er, pre_Er;
int16_t sum_Er, G_temp;
uint8_t GAB = 30;

void loop(void) {
	loop_pid1();
}

void loop_pid1(void) {
	encoder_update();
	LS_update;

	if (LSread) {
		pid1();
		if (Last_LSread != LSread)
			Last_LSread = LSread;
	} else {
		sum_Er = 0;
		if (Last_LSread & LEFT) {
			GA = -35;
			GB = 15;
		} else if (Last_LSread & RIGHT) {
			GA = 15;
			GB = -35;
		}
	}

	pid0();
}

void pid0(void) {

	MOTOR(SPDA, SPDB);

	pre_PA = PA;
	pre_PB = PB;
	PA = GA - ecdA;
	PB = GB - ecdB;
	sum_PA += PA;
	sum_PB += PB;

	SPDA = 13 * PA + 1.5 * sum_PA - 2 * (PA - pre_PA);
	SPDB = 13 * PB + 1.5 * sum_PB - 2 * (PB - pre_PB);
}

void pid1(void) {
	pre_Er = Er;
	Er = 0;

	for (uint8_t i = 0; i < 4; i++) {
		if (LSread & (1 << i))
			Er += jq[i];
		if (LSread & (1 << (i + 4)))
			Er -= jq[i];
	}

	if (sum_Er < GAB * 100 && sum_Er > GAB * -100)
		sum_Er += Er;
	G_temp = Kp * Er + Ki * sum_Er + Kd * (Er - pre_Er);
	G_temp /= 20;

	GA = GAB + G_temp;
	GB = GAB - G_temp;

	GA = (-20 < GA && GA < 70) ? GA : ((GA < 0) ? -20 : 70);
	GB = (-20 < GB && GB < 70) ? GB : ((GB < 0) ? -20 : 70);
}

void OLED_loop(void) {
	OLED_ShowString(0, 0, "SPDA", 8, 1);
	OLED_ShowNumNoLen(0, 8, SPDA, 8, 1);
	OLED_ShowNumNoLen(0, 8 + 8 + 5, GA, 16, 1);
	OLED_ShowNumNoLen(0, 8 + 8 + 5 + 16 + 3, ecdA, 16, 1);
	OLED_ShowString(0, 8 + 8, "GA", 8, 1);
	OLED_ShowString(0, 8 + 8 + 5 + 14, "ecdA", 8, 1);

	OLED_ShowString(63, 0, "SPDB", 8, 1);
	OLED_ShowNumNoLen(63, 8, SPDB, 8, 1);
	OLED_ShowNumNoLen(63, 8 + 8 + 5, GB, 16, 1);
	OLED_ShowNumNoLen(63, 8 + 8 + 5 + 16 + 3, ecdB, 16, 1);
	OLED_ShowString(63, 8 + 8, "GB", 8, 1);
	OLED_ShowString(63, 8 + 8 + 5 + 14, "ecdB", 8, 1);

	// OLED_ShowNumNoLen(0, 8 + 8 + 5, ultrasound_distance(), 16, 1);

	OLED_Refresh();
	OLED_ClearRF();
}

/*
 * 字体大小：16
 * 行间隔：15
 *
 * 字体大小：8
 * 行间隔：8
 */
