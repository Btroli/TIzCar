#include "ti_msp_dl_config.h"

#include "AllHeader.h"

int main(void) {
	SYSCFG_DL_init();

	Motor_Init();

	encoder_init();


	while (1) {

		encoder_update();

		delay_cycles(1600000);
	}
}
