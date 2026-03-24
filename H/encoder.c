#include "encoder.h"

volatile  ENCODER_RES motorL_encoder;
volatile  ENCODER_RES motorR_encoder;

int g_Encoder_M1_Now = 0;
int g_Encoder_M2_Now = 0;


volatile u8 encoder_buf[64];
//编码器初始化
void encoder_init(void) {
	//编码器引脚外部中断
	NVIC_ClearPendingIRQ(GPIOB_INT_IRQn);
	NVIC_ClearPendingIRQ(GPIOA_INT_IRQn);
	NVIC_EnableIRQ(GPIOB_INT_IRQn);
	NVIC_EnableIRQ(GPIOA_INT_IRQn);

	//测速定时器
	// Timer_20ms_Init();
}


//获取左边编码器的方向
ENCODER_DIR get_encoderL_dir(void) {
	return motorL_encoder.dir;
}



//获取右边编码器的方向
ENCODER_DIR get_encoderR_dir(void) {
	return motorR_encoder.dir;
}


//获取开机到现在总共的四路编码器计数。
void Encoder_Get_ALL(int *Encoder_all) {
	Encoder_all[0] = motorL_encoder.ALLcount;
	Encoder_all[1] = motorR_encoder.ALLcount;
}

//获取每x ms的电机值 这里看测速定时器决定
void Encoder_Get_Temp(int *Encoder_temp) {
	Encoder_temp[0] = motorL_encoder.ALLcount;
	Encoder_temp[1] = motorR_encoder.ALLcount;
}



//编码器数据更新
//请间隔一定时间更新 20ms更新一次
void encoder_update(void) {
	motorL_encoder.count = motorL_encoder.temp_count;
	motorR_encoder.count = motorR_encoder.temp_count;

	motorL_encoder.ALLcount += motorL_encoder.temp_count;
	motorR_encoder.ALLcount += motorR_encoder.temp_count;

	//确定方向
	motorL_encoder.dir = ( motorL_encoder.count >= 0 ) ? FORWARD : REVERSAL;
	motorR_encoder.dir = ( motorR_encoder.count >= 0 ) ? FORWARD : REVERSAL;

	;

	motorL_encoder.temp_count = 0;//编码器计数值清零
	motorR_encoder.temp_count = 0;
}
// 外部中断处理函数
void GROUP1_IRQHandler(void) {

    uint32_t pendingSource = DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1);
    
    // ---------- GPIOB 中断 ----------
    if (pendingSource & DL_INTERRUPT_GROUP1_IIDX_GPIOB) {
        // 获取 GPIOB 上所有已使能中断引脚的状态（我们只关心 H1B 和 H2A）
        uint32_t gpio_status = DL_GPIO_getEnabledInterruptStatus(GPIOB, 
                                    GPIO_ENCODER_L_H1B_PIN | GPIO_ENCODER_R_H2A_PIN);
        
        // 处理左电机 H1B (B相) 中断
        if (gpio_status & GPIO_ENCODER_L_H1B_PIN) {
            // B相上升沿，根据 A相 电平决定方向
            // A相现在是 GPIOA.21
            if (!DL_GPIO_readPins(GPIOA, GPIO_ENCODER_L_H1A_PIN)) {
                motorL_encoder.temp_count--;   // A=0 -> 反向
            } else {
                motorL_encoder.temp_count++;   // A=1 -> 正向
            }
            DL_GPIO_clearInterruptStatus(GPIOB, GPIO_ENCODER_L_H1B_PIN);
        }
        
        // 处理右电机 H2A (A相) 中断
        if (gpio_status & GPIO_ENCODER_R_H2A_PIN) {
            // A相上升沿，根据 B相 电平决定方向
            // B相是 GPIOA.31
            if (DL_GPIO_readPins(GPIOA, GPIO_ENCODER_R_H2B_PIN)) {
                motorR_encoder.temp_count++;   // B=1 -> 正向
            } else {
                motorR_encoder.temp_count--;   // B=0 -> 反向
            }
            DL_GPIO_clearInterruptStatus(GPIOB, GPIO_ENCODER_R_H2A_PIN);
        }
    }
    
    // ---------- GPIOA 中断 ----------
    if (pendingSource & DL_INTERRUPT_GROUP1_IIDX_GPIOA) {
        // 获取 GPIOA 上所有已使能中断引脚的状态（我们只关心 H1A 和 H2B）
        uint32_t gpio_status = DL_GPIO_getEnabledInterruptStatus(GPIOA,
                                    GPIO_ENCODER_L_H1A_PIN | GPIO_ENCODER_R_H2B_PIN);
        
        // 处理左电机 H1A (A相) 中断
        if (gpio_status & GPIO_ENCODER_L_H1A_PIN) {
            // A相上升沿，根据 B相 电平决定方向
            // B相是 GPIOB.22
            if (!DL_GPIO_readPins(GPIOB, GPIO_ENCODER_L_H1B_PIN)) {
                motorL_encoder.temp_count++;   // B=0 -> 正向
            } else {
                motorL_encoder.temp_count--;   // B=1 -> 反向
            }
            DL_GPIO_clearInterruptStatus(GPIOA, GPIO_ENCODER_L_H1A_PIN);
        }
        
        // 处理右电机 H2B (B相) 中断
        if (gpio_status & GPIO_ENCODER_R_H2B_PIN) {
            // B相上升沿，根据 A相 电平决定方向
            // A相是 GPIOB.13
            if (DL_GPIO_readPins(GPIOB, GPIO_ENCODER_R_H2A_PIN)) {
                motorR_encoder.temp_count--;   // A=1 -> 反向
            } else {
                motorR_encoder.temp_count++;   // A=0 -> 正向
            }
            DL_GPIO_clearInterruptStatus(GPIOA, GPIO_ENCODER_R_H2B_PIN);
        }
    }
}
