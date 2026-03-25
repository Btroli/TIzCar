#include "encoder.h"

volatile int16_t ecdA, ecdB;

volatile int16_t ECDA_temp, ECDB_temp;

void encoder_init(void) {
	//编码器引脚外部中断
	NVIC_ClearPendingIRQ(GPIOB_INT_IRQn);
	NVIC_ClearPendingIRQ(GPIOA_INT_IRQn);
	NVIC_EnableIRQ(GPIOB_INT_IRQn);
	NVIC_EnableIRQ(GPIOA_INT_IRQn);

	// Timer_20ms_Init();
}

//编码器数据更新，20ms更新一次
void encoder_update(void) {
	ecdA = ECDA_temp;
	ecdB = ECDB_temp;

	ECDA_temp = 0;
	ECDB_temp = 0;
}

// 外部中断处理函数
void GROUP1_IRQHandler(void) {
    uint32_t pending = DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1);
    
    if (pending & DL_INTERRUPT_GROUP1_IIDX_GPIOB) {
        uint32_t status = DL_GPIO_getEnabledInterruptStatus(GPIOB,
                            GPIO_ENCODER_L_E1B_PIN | GPIO_ENCODER_R_E2A_PIN);
        if (status & GPIO_ENCODER_L_E1B_PIN) {
            if (DL_GPIO_readPins(GPIOA, GPIO_ENCODER_L_E1A_PIN))
                ECDA_temp--;
            else
                ECDA_temp++;
            DL_GPIO_clearInterruptStatus(GPIOB, GPIO_ENCODER_L_E1B_PIN);
        }
        if (status & GPIO_ENCODER_R_E2A_PIN) {
            if (DL_GPIO_readPins(GPIOA, GPIO_ENCODER_R_E2B_PIN))
                ECDB_temp--;
            else
                ECDB_temp++;
            DL_GPIO_clearInterruptStatus(GPIOB, GPIO_ENCODER_R_E2A_PIN);
        }
    }
    
    if (pending & DL_INTERRUPT_GROUP1_IIDX_GPIOA) {
        uint32_t status = DL_GPIO_getEnabledInterruptStatus(GPIOA,
                            GPIO_ENCODER_L_E1A_PIN | GPIO_ENCODER_R_E2B_PIN);
        if (status & GPIO_ENCODER_L_E1A_PIN) {
            if (DL_GPIO_readPins(GPIOB, GPIO_ENCODER_L_E1B_PIN))
                ECDA_temp++;
            else
                ECDA_temp--;
            DL_GPIO_clearInterruptStatus(GPIOA, GPIO_ENCODER_L_E1A_PIN);
        }
        if (status & GPIO_ENCODER_R_E2B_PIN) {
            if (DL_GPIO_readPins(GPIOB, GPIO_ENCODER_R_E2A_PIN))
                ECDB_temp++;
            else
                ECDB_temp--;
            DL_GPIO_clearInterruptStatus(GPIOA, GPIO_ENCODER_R_E2B_PIN);
        }
    }
}
