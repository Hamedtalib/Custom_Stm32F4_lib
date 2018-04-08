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
#define ENCODER_GPIO_PIN	GPIO_Pin_10
#define ENCODER_GPIO_CLK	RCC_AHB1Periph_GPIOA
#define ENCODER_TIM	        TIM1
#define ENCODER_TIM_CLK	        RCC_APB1Periph_TIM1
#define ENCODER_TIM_PIN	        GPIO_PinSource10
#define ENCODER_TIM_AF	        GPIO_AF_TIM1
#define ENCODER_TIM_CHANNEL	TIM_Channel_3
#define ENCODER_TIM_IRQ	        TIM1_IRQn
#define ENCODER_TIM_IT          TIM_IT_CC3
#define ENCODER_TIM_IRQ_HANDLE	TIM1_IRQHandler

   
/* Public functions ---------------------------------------------------------*/
void encoder_init(void);
uint32_t encoder_get_frequency(void);

/* Private functions ---------------------------------------------------------*/
