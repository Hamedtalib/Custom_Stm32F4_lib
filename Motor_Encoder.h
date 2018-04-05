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
#define ENCODER_GPIO_PIN	GPIO_Pin_0
#define ENCODER_GPIO_CLK	RCC_AHB1Periph_GPIOA
#define ENCODER_TIM	TIM5
#define ENCODER_TIM_CLK	RCC_APB1Periph_TIM5
#define ENCODER_TIM_PIN	GPIO_PinSource0
#define ENCODER_TIM_AF	GPIO_AF_TIM5
#define ENCODER_TIM_CHANNEL	TIM_Channel_1
#define ENCODER_TIM_IRQ	TIM5_IRQn
#define ENCODER_TIM_IT     TIM_IT_CC1
#define ENCODER_TIM_IRQ_HANDLE	TIM5_IRQHandler

   
/* Public functions ---------------------------------------------------------*/
void encoder_init(void);
uint32_t encoder_get_frequency(void);

/* Private functions ---------------------------------------------------------*/
