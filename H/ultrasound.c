#include "ultrasound.h"

volatile int8_t  capture_done = 1;
volatile uint16_t  echotime = 0xFFFF;

/* 
 * ע�⣺���� ti_msp_dl_config.h �Ķ��壬
 * CAPTURE_ULTRASOUND_ECHO_INST ��Ϊ TIMG0
 * �жϺ����������� SysConfig ���ɵ�һ��
 */
// void CAPTURE_ULTRASOUND_ECHO_INST_IRQHandler(void)
// {
//     /* 
//      * 1. ���������ж� (��Ӧ STM32 �� TIM_IT_CC4)
//      * ʹ�� CC0_DN_EVENT ��Ϊ�����¼���־
//      */
//     if (DL_Timer_getRawInterruptStatus(CAPTURE_ULTRASOUND_ECHO_INST, 
//                                        DL_TIMER_INTERRUPT_CC0_DN_EVENT)) 
//     {
//         // ����жϱ�־
//         DL_Timer_clearInterruptStatus(CAPTURE_ULTRASOUND_ECHO_INST, 
//                                       DL_TIMER_INTERRUPT_CC0_DN_EVENT);

//         // ��ȡ��ǰ����������ã��жϵ�ǰ�������ػ����½��ش���
//         // ��Ӧ STM32: if (TIM5->CCER & TIM_CCER_CC4P)
//         uint32_t currentConfig = DL_Timer_getCaptureCompareCtl(CAPTURE_ULTRASOUND_ECHO_INST, DL_TIMER_CC_0_INDEX);

//         // ����Ƿ�����Ϊ�½��ز���
//         if (currentConfig & DL_TIMER_CC_CCOND_TRIG_FALL) 
//         {
//             /* --- �½��ز��� (��������) --- */
            
//             // ��ȡ����ֵ
//             echotime = DL_Timer_getCaptureCompareValue(CAPTURE_ULTRASOUND_ECHO_INST, DL_TIMER_CC_0_INDEX);
            
//             // �л��������ز���Ϊ��һ�β�����׼��
//             DL_Timer_setCaptureCompareCtl(CAPTURE_ULTRASOUND_ECHO_INST, 
//                                           DL_TIMER_CC_MODE_CAPTURE, 
//                                           DL_TIMER_CC_CCOND_TRIG_RISE, 
//                                           DL_TIMER_CC_0_INDEX);
            
//             capture_done = 1; // ��ǲ������
//         }
//         else 
//         {
//             /* --- �����ز��� (������ʼ) --- */
            
//             // ���������
//             DL_Timer_setTimerCount(CAPTURE_ULTRASOUND_ECHO_INST, 0);
            
//             // �л�Ϊ�½��ز���
//             DL_Timer_setCaptureCompareCtl(CAPTURE_ULTRASOUND_ECHO_INST, 
//                                           DL_TIMER_CC_MODE_CAPTURE, 
//                                           DL_TIMER_CC_CCOND_TRIG_FALL, 
//                                           DL_TIMER_CC_0_INDEX);
            
//             capture_done = -1; // ������ڼ�ʱ
//         }
//     }

//     /* 
//      * 2. ��������ж� (��Ӧ STM32 �� TIM_IT_Update) 
//      * ���ڳ�ʱ����
//      */
//     if (DL_Timer_getRawInterruptStatus(CAPTURE_ULTRASOUND_ECHO_INST, 
//                                        DL_TIMER_INTERRUPT_OVERFLOW_EVENT)) 
//     {
//         DL_Timer_clearInterruptStatus(CAPTURE_ULTRASOUND_ECHO_INST, 
//                                       DL_TIMER_INTERRUPT_OVERFLOW_EVENT);
        
//         if (capture_done == -1) 
//         {
//             echotime = 0xFFFF;
//             capture_done = 1;
//         }
//     }
// }


// void CAPTURE_ULTRASOUND_ECHO_INST_IRQHandler(void)
// {
//     /* 1. ���������ж� */
//     if (DL_Timer_getRawInterruptStatus(CAPTURE_ULTRASOUND_ECHO_INST, 
//                                        DL_TIMER_INTERRUPT_CC0_DN_EVENT)) 
//     {
//         DL_Timer_clearInterruptStatus(CAPTURE_ULTRASOUND_ECHO_INST, 
//                                       DL_TIMER_INTERRUPT_CC0_DN_EVENT);

//         uint32_t currentConfig = DL_Timer_getCaptureCompareCtl(CAPTURE_ULTRASOUND_ECHO_INST, DL_TIMER_CC_0_INDEX);

//         // ��鵱ǰ�Ƿ�Ϊ�½��ز���ģʽ
//         if (currentConfig & DL_TIMER_CC_CCOND_TRIG_FALL) 
//         {
//             /* --- �½��ز��� (��������) --- */
//             echotime = DL_Timer_getCaptureCompareValue(CAPTURE_ULTRASOUND_ECHO_INST, DL_TIMER_CC_0_INDEX);
            
//             // �л��������ز���
//             // ע�⣺������� DL_TIMER_CC_ACOND_TIMCLK������ʱ����ֹͣ������
//             DL_Timer_setCaptureCompareCtl(CAPTURE_ULTRASOUND_ECHO_INST, 
//                                           DL_TIMER_CC_MODE_CAPTURE, 
//                                           (DL_TIMER_CC_ACOND_TIMCLK | DL_TIMER_CC_CCOND_TRIG_RISE), 
//                                           DL_TIMER_CC_0_INDEX);
            
//             capture_done = 1; 
//         }
//         else 
//         {
//             /* --- �����ز��� (������ʼ) --- */
//             DL_Timer_setTimerCount(CAPTURE_ULTRASOUND_ECHO_INST, 0);
            
//             // �л�Ϊ�½��ز���
//             // ע�⣺������� DL_TIMER_CC_ACOND_TIMCLK
//             DL_Timer_setCaptureCompareCtl(CAPTURE_ULTRASOUND_ECHO_INST, 
//                                           DL_TIMER_CC_MODE_CAPTURE, 
//                                           (DL_TIMER_CC_ACOND_TIMCLK | DL_TIMER_CC_CCOND_TRIG_FALL), 
//                                           DL_TIMER_CC_0_INDEX);
            
//             capture_done = -1; 
//         }
//     }

//     /* 2. ��������ж� */
//     if (DL_Timer_getRawInterruptStatus(CAPTURE_ULTRASOUND_ECHO_INST, 
//                                        DL_TIMER_INTERRUPT_OVERFLOW_EVENT)) 
//     {
//         DL_Timer_clearInterruptStatus(CAPTURE_ULTRASOUND_ECHO_INST, 
//                                       DL_TIMER_INTERRUPT_OVERFLOW_EVENT);
        
//         if (capture_done == -1) 
//         {
//             echotime = 0xFFFF;
//             capture_done = 1;
//         }
//     }
// }

int testAAA;

// void CAPTURE_ULTRASOUND_ECHO_INST_IRQHandler(void)
// {
//     testAAA++;
//     /* 
//      * 1. �����������ж� (������ʼ)
//      * ��Ӧ SysConfig �е� "Channel 0 compare up event"
//      */
//     if (DL_Timer_getRawInterruptStatus(CAPTURE_ULTRASOUND_ECHO_INST, 
//                                        DL_TIMER_INTERRUPT_CC0_UP_EVENT)) 
//     {
//         // ����жϱ�־
//         DL_Timer_clearInterruptStatus(CAPTURE_ULTRASOUND_ECHO_INST, 
//                                       DL_TIMER_INTERRUPT_CC0_UP_EVENT);

//         // �������������ʼ��ʱ
//         DL_Timer_setTimerCount(CAPTURE_ULTRASOUND_ECHO_INST, 0);
        
//         // ������ڼ�ʱ
//         capture_done = -1; 
//     }

//     /* 
//      * 2. �����½����ж� (��������)
//      * ��Ӧ SysConfig �е� "Channel 0 compare down event"
//      */
//     if (DL_Timer_getRawInterruptStatus(CAPTURE_ULTRASOUND_ECHO_INST, 
//                                        DL_TIMER_INTERRUPT_CC0_DN_EVENT)) 
//     {
//         // ����жϱ�־
//         DL_Timer_clearInterruptStatus(CAPTURE_ULTRASOUND_ECHO_INST, 
//                                       DL_TIMER_INTERRUPT_CC0_DN_EVENT);

//         // ��ȡ����ֵ
//         echotime = DL_Timer_getCaptureCompareValue(CAPTURE_ULTRASOUND_ECHO_INST, DL_TIMER_CC_0_INDEX);
        
//         // ��ǲ������
//         capture_done = 1; 
//     }

//     /* 
//      * 3. ��������ж� (��ʱ����)
//      */
//     if (DL_Timer_getRawInterruptStatus(CAPTURE_ULTRASOUND_ECHO_INST, 
//                                        DL_TIMER_INTERRUPT_OVERFLOW_EVENT)) 
//     {
//         DL_Timer_clearInterruptStatus(CAPTURE_ULTRASOUND_ECHO_INST, 
//                                       DL_TIMER_INTERRUPT_OVERFLOW_EVENT);
        
//         if (capture_done == -1) 
//         {
//             echotime = 0xFFFF;
//             capture_done = 1;
//         }
//     }
// }

void CAPTURE_ULTRASOUND_ECHO_INST_IRQHandler(void)
{
    testAAA++; // 保留这个用于调试

    // 1. 只处理 Capture Up Event (对应上升沿或双边沿触发)
    if (DL_Timer_getRawInterruptStatus(CAPTURE_ULTRASOUND_ECHO_INST, DL_TIMER_INTERRUPT_CC0_UP_EVENT))
    {
        DL_Timer_clearInterruptStatus(CAPTURE_ULTRASOUND_ECHO_INST, DL_TIMER_INTERRUPT_CC0_UP_EVENT);
        
        // 读取捕获值
        uint16_t currentCount = DL_Timer_getCaptureCompareValue(CAPTURE_ULTRASOUND_ECHO_INST, DL_TIMER_CC_0_INDEX);
        
        // 2. 判断状态机
        if (capture_done == 0) 
        {
            // 状态0: 等待测量开始 (此时应该是高电平到来)
            // 记录起始时间或直接清零计数器
            DL_Timer_setTimerCount(CAPTURE_ULTRASOUND_ECHO_INST, 0); 
            capture_done = -1; // 进入计时状态
        }
        else if (capture_done == -1) 
        {
            // 状态-1: 捕获到了结束边沿 (下降沿)
            echotime = currentCount; // 直接保存计数值
            capture_done = 1;        // 测量完成！
            // 注意：这里不再去修改硬件的边沿检测模式
        }
    }
    
    // 删除溢出处理部分
}

float ultrasound_distance(void) {
	return echotime * 0.017;
}
