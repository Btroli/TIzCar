#include "ti_msp_dl_config.h"

#include "AllHeader.h"

#define TOG(gpio,pins) DL_GPIO_togglePins(gpio,pins)

int main(void)
{
    SYSCFG_DL_init();

	while (1) {

		TOG(GPIOA,LED_MCU_PIN);

		delay_cycles(80000000);
	}
}
