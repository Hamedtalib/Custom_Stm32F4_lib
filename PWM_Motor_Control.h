/**
 * Driver for PWM Motor
 *
 * @author      Tyler Thompson
 * @date        4/5/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define __PWM_MOTOR_CONTROL_H

#define MOTOR_GPIO_PORT	        GPIOD
#define MOTOR_GPIO_PIN	        GPIO_Pin_14
#define MOTOR_GPIO_CLK	        RCC_AHB1Periph_GPIOD
#define MOTOR_TIM	        TIM4
#define MOTOR_TIM_CLK	        RCC_APB1Periph_TIM4
#define MOTOR_TIM_PIN	        GPIO_PinSource14
#define MOTOR_TIM_AF	        GPIO_AF_TIM4
#define MOTOR_PWM_CHANNEL	TIM_OCMode_PWM1
#define MOTOR_TIM_CHANNEL	TIM_Channel_3
#define MOTOR_OC_INIT           TIM_OC3Init

/* Private variables ---------------------------------------------------------*/


/* Public functions ---------------------------------------------------------*/
void pwmmotor_init(void);
void pwmmotor_set_duty(uint32_t duty);
void pwmmotor_enable(void);
void pwmmotor_disable(void);

/* Private functions ---------------------------------------------------------*/
