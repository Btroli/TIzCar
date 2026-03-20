#include "ti_msp_dl_config.h"

#include "AllHeader.h"

int main(void) {
	SYSCFG_DL_init();

	OLED_SET();

	volatile uint8_t read;

	for (uint8_t i = 0; i < 8; i++)
		OLED_ShowChar(3 + 15 * i, 2, '1' + i, 12, 1);

	while (1) {
		OLED_ClearRF();

		// read = LS_READ;

		// for (uint8_t i = 0; i < 8; i++) {
		// 	OLED_DrawBox(15 * i, 18, 12 + 15 * i, 30, 1);
		// 	if (1 ^ read >> i & 1)
		// 		OLED_DrawBox(15 * i + 1, 19, 11 + 15 * i, 29, 0);
		// }

		if (ReadKEY1)
			OLED_DrawBox(15 * 5, 18, 12 + 15 * 5, 30, 1);
		else
			OLED_DrawBox(15 * 6, 18, 12 + 15 * 6, 30, 1);

		OLED_Refresh();

		delay_cycles(800000);
	}
}
