/**
 * Driver for PWM Motor
 *
 * @author      Tyler Thompson
 * @date        4/5/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define __PWM_MOTOR_CONTROL_H

/* Forward motor signal variables */
#define MOTORF_GPIO_PORT	GPIOD
#define MOTORF_GPIO_PIN	        GPIO_Pin_14
#define MOTORF_GPIO_CLK	        RCC_AHB1Periph_GPIOD
#define MOTORF_TIM	        TIM4
#define MOTORF_TIM_CLK	        RCC_APB1Periph_TIM4
#define MOTORF_TIM_PIN	        GPIO_PinSource14
#define MOTORF_TIM_AF	        GPIO_AF_TIM4
#define MOTORF_PWM_CHANNEL	TIM_OCMode_PWM1
#define MOTORF_TIM_CHANNEL	TIM_Channel_3
#define MOTORF_OC_INIT           TIM_OC3Init
#define MOTORF_INIT_FREQ        ((uint32_t) 30000)
#define MOTORF_INIT_DUTY        ((uint32_t) 50)

/* Reverse motor signal variables */
#define MOTORR_GPIO_PORT	GPIOA
#define MOTORR_GPIO_PIN	        GPIO_Pin_2
#define MOTORR_GPIO_CLK	        RCC_AHB1Periph_GPIOA
#define MOTORR_TIM	        TIM9
#define MOTORR_TIM_CLK	        RCC_APB2Periph_TIM9
#define MOTORR_TIM_PIN	        GPIO_PinSource2
#define MOTORR_TIM_AF	        GPIO_AF_TIM9
#define MOTORR_PWM_CHANNEL	TIM_OCMode_PWM1
#define MOTORR_TIM_CHANNEL	TIM_Channel_1
#define MOTORR_OC_INIT           TIM_OC1Init
#define MOTORR_INIT_FREQ        ((uint32_t) 30000)
#define MOTORR_INIT_DUTY        ((uint32_t) 50)

/* Private variables ---------------------------------------------------------*/


/* Public functions ---------------------------------------------------------*/
void pwmmotor_init(void);
void pwmmotor_set_duty(uint32_t duty, uint8_t direction);
void pwmmotor_enable(uint8_t direction);
void pwmmotor_disable(uint8_t direction);

/* Private functions ---------------------------------------------------------*/
static void __forward_init(void);
static void __reverse_init(void);