#include "ti_msp_dl_config.h"

#include "AllHeader.h"

int main(void) {
	SYSCFG_DL_init();

	Motor_init();
	encoder_init();
	TIM_init();

	while (1);
}

void loop(void) {
	encoder_update();
	LS_update;
}
