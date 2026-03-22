#include "ti_msp_dl_config.h"

#include "AllHeader.h"

int main(void) {
	SYSCFG_DL_init();

	Motor_Init();

	while (1) {

		pwmA(500);
		pwmB(500);

		delay_cycles(800000);
	}
}
