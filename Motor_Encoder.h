/**
 * Driver for motor encoder
 *
 * @author      Tyler Thompson
 * @date        4/5/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define __MOTOR_ENCODER_H

#define ENCODER_GPIO_PORT	GPIOA
#define ENCODER_GPIO_PIN	GPIO_Pin_2
#define ENCODER_GPIO_CLK	RCC_AHB1Periph_GPIOA
#define ENCODER_TIM	        TIM9
#define ENCODER_TIM_CLK	        RCC_APB2Periph_TIM9
#define ENCODER_TIM_PIN	        GPIO_PinSource2
#define ENCODER_TIM_AF	        GPIO_AF_TIM9
#define ENCODER_TIM_CHANNEL	TIM_Channel_1
#define ENCODER_TIM_IRQ	        TIM1_BRK_TIM9_IRQn
#define ENCODER_TIM_IT          TIM_IT_CC1
#define ENCODER_TIM_IRQ_HANDLE	TIM9_IRQHandler

   
/* Public functions ---------------------------------------------------------*/
void encoder_init(void);
uint32_t encoder_get_frequency(void);
void ENCODER_TIM_IRQ_HANDLE(void);
/* Private functions ---------------------------------------------------------*/
