#include "ti_msp_dl_config.h"

#include "AllHeader.h"

void pid0(void);
void pid1(void);
void OLED_loop(void);

void loop_pid1(void);
void loop_quan(void);
void loop_bz(void);

int main(void) {
	SYSCFG_DL_init();

	OLED_SET();

	Motor_init();
	encoder_init();
	TIM_init();
	Ultrasound_init();

	while (1);
}


//loop
uint8_t Last_LSread;
int8_t GA = 0, GB = 0;

//pid0
int16_t SPDA = 0, SPDB = 0, PA = 0, PB = 0, pre_PA = 0, pre_PB = 0, sum_PA = 0, sum_PB = 0;

//pid1

static const int8_t jq[4] = {15, 20, 12, 8};
volatile float Kp = 60, Ki = 2, Kd = 35;
int8_t Er, pre_Er;
int16_t sum_Er, G_temp;
uint8_t GAB = 30;

void loop(void) {
	if (value[0][0])
		loop_pid1();
	else if (value[1][0])
		loop_quan();
	else if (value[2][0])
		loop_bz();
	else
		;
}

int8_t bz_mode = 0, bz_turn;
volatile int16_t bz_time = 0;
typedef enum {
	BZ_IDLE = 0,	// 正常循迹
	BZ_LEFT,		// 左转
	BZ_FWD,			// 向前开
	BZ_RIGHT,		// 右转
	BZ_FWD2			// 向右转后向前开，等待碰线
} BZ_State;
volatile BZ_State bz_state = BZ_IDLE;

void loop_bz(void) {
	encoder_update();
	LS_update;

	if (bz_state == BZ_IDLE) {
		if (ultrasound_distance() < 23) {	//改为23cm
			STBY_0;
			bz_time = 0;
			bz_state = BZ_LEFT;
			return;
		}

		if (LSread) {
			pid1();
			if (Last_LSread != LSread)
				Last_LSread = LSread;
		} else {
			sum_Er = 0;
			if (Last_LSread & LEFT) {
				GA = -10;
				GB = 40;
			} else if (Last_LSread & RIGHT) {
				GA = 40;
				GB = -10;
			}
		}
	}

	if (bz_state != BZ_IDLE) {
		bz_time++;		//计时+1（约10ms/次）

		switch (bz_state) {

			case BZ_LEFT:
				STBY_1;
				GA = -10;
				GB = 40;
				if (bz_time >= 15) {		//左转 ≈ 300ms（20ms×15）
					GA = 40;
					GB = 40;
					bz_time = 0;
					bz_state = BZ_FWD;
				}
				break;

			case BZ_FWD:
				STBY_1;				//确保电机使能
				GA = 40;			//加大速度，确保能前进
				GB = 40;
				if (bz_time >= 40) {		//直行 ≈ 1秒（20ms×50）
					GA = 40;
					GB = -10;
					bz_time = 0;
					bz_state = BZ_RIGHT;
					return;
				}
				break;

			case BZ_RIGHT:

				GA = 40;
				GB = -10;
				if (bz_time >= 32) {		//右转 ≈ 460ms（20ms×23）
					GA = 40;
					GB = 40;
					bz_time = 0;
					bz_state = BZ_FWD2;
					return;			//本次loop到此为止
				}
				break;

			case BZ_FWD2:
				GA = 40;
				GB = 40;
				if (LSread & 0x3E) {
					bz_state = BZ_IDLE;
				}
				break;
			case BZ_IDLE:
				break;
		}

		SPDA = GA * 10;
		SPDB = GB * 10;
		MOTOR(SPDA, SPDB);
		return;
	}

	pid0();
}

//stop
uint32_t distance = 0, distance_now = 0, distance_pj = 0;
uint8_t bian = 0, bian_pre = 0, bian_flag = 1;
uint8_t quan = 3;
volatile uint32_t Stime_bian = 0xFFFFFFFF + 1 - 200;	//2^32 - 200

void loop_quan(void) {
	encoder_update();
	LS_update;

	if (LSread) {
		pid1();
		if (Last_LSread != LSread)
			Last_LSread = LSread;
		bian_flag = 1;
	} else {
		sum_Er = 0;
		if (Last_LSread & LEFT) {
			GA = -35;
			GB = 15;
		} else if (Last_LSread & RIGHT) {
			GA = 15;
			GB = -35;
		}

		if (bian_flag && Stime - Stime_bian >= 50) {	//此处防止转弯不稳定
			bian++;
			Stime_bian = Stime;
		}
		bian_flag = 0;
	}

	pid0();

	distance_now += (ecdA + ecdB) / 2;

	if (bian != bian_pre) {
		bian_pre = bian;

		if (bian == 1)
			distance = distance_now;
		else if (bian == 4 * quan) {
			distance_pj += distance_now;
			distance_pj /= (4 * quan - 1);
			distance = distance_pj - distance;
		} else
			distance_pj += distance_now;
		distance_now = 0;
	}

	if (bian == 4 * quan)
		if (distance_now >= distance) {
			Motor_Stop(1);
			while (1);
			// while (1)
			// 	if (ReadKEY1) {
			// 		bian = 0;
			// 		bian_pre = 0;
			// 		bian_flag = 1;
			// 		quan = 8;
			// 		Stime_bian = 0xFFFFFFFF + 1 - 200;
			// 		Stime = 0;
			// 		return;
			// 	}
		}
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
			GA = -10;
			GB = 40;
		} else if (Last_LSread & RIGHT) {
			GA = 40;
			GB = -10;
		}
	}

	pid0();
}

void pid0(void) {

	pre_PA = PA;
	pre_PB = PB;
	PA = GA - ecdA;
	PB = GB - ecdB;
	sum_PA += PA;
	sum_PB += PB;
	if (sum_PA > 800)
		sum_PA = 800;
	if (sum_PA < -800)
		sum_PA = -800;
	if (sum_PB > 800)
		sum_PB = 800;
	if (sum_PB < -800)
		sum_PB = -800;

	SPDA = 13 * PA + 3 * sum_PA / 2 - 2 * (PA - pre_PA);
	SPDB = 13 * PB + 3 * sum_PB / 2 - 2 * (PB - pre_PB);
	if (SPDA >  1000)
		SPDA =  1000;
	if (SPDA < -1000)
		SPDA = -1000;
	if (SPDB >  1000)
		SPDB =  1000;
	if (SPDB < -1000)
		SPDB = -1000;
	MOTOR(SPDA, SPDB);
}

void pid1(void) {
	pre_Er = Er;
	Er = 0;

	for (uint8_t i = 0; i < 4; i++) {
		if (LSread & (1 << i))
			Er += jq[i];
		if (LSread & (1 << (7 - i)))
			Er -= jq[i];
	}

	if (sum_Er < GAB * 100 && sum_Er > GAB * -100)
		sum_Er += Er;
	G_temp = Kp * Er + Ki * sum_Er + Kd * (Er - pre_Er);
	G_temp /= 200;

	GA = GAB + G_temp;
	GB = GAB - G_temp;

	GA = (-20 < GA && GA < 70) ? GA : ((GA < 0) ? -20 : 70);
	GB = (-20 < GB && GB < 70) ? GB : ((GB < 0) ? -20 : 70);
}

void OLED_loop(void) {

	if (LoopMode == Loop_CHOICE) {
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

		OLED_Refresh();
		OLED_ClearRF();
		if (KEY_Scan1() || KEY_Scan2() || KEY_Scan3() || KEY_Scan4())
			LoopMode = Loop_SCREEN0;
	}


	else if (LoopMode == Loop_SCREEN0)
		loop_screen0();
	else if (LoopMode == Loop_SCREEN1)
		loop_screen1();

}

/*
 * 字体大小：16
 * 行间隔：15
 *
 * 字体大小：8
 * 行间隔：8
 */

