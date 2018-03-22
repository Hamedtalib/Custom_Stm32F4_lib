/**
 * Driver for Ultra Sonic Sensor
 * Model: HC-SR4
 *
 * @author      Tyler Thompson
 * @date        3/20/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "Timing_Delay.h"

#define __ULTRASONICSENSOR_H

#define ECHO_GPIO_PORT	GPIOA
#define ECHO_GPIO_PIN	GPIO_Pin_0
#define ECHO_GPIO_CLK	RCC_AHB1Periph_GPIOA
#define ECHO_TIM	TIM5
#define ECHO_TIM_CLK	RCC_APB1Periph_TIM5
#define ECHO_TIM_PIN	GPIO_PinSource0
#define ECHO_TIM_AF	GPIO_AF_TIM5
#define ECHO_TIM_CHANNEL	TIM_Channel_1
#define ECHO_TIM_IRQ	TIM5_IRQn
#define ECHO_TIM_IT     TIM_IT_CC1

#define TRIG_GPIO_PORT	GPIOB
#define TRIG_GPIO_PIN	GPIO_Pin_10
#define TRIG_GPIO_CLK	RCC_AHB1Periph_GPIOB
#define TRIG_TIM	TIM2
#define TRIG_TIM_CLK	RCC_APB1Periph_TIM2
#define TRIG_TIM_PIN	GPIO_PinSource10
#define TRIG_TIM_AF	GPIO_AF_TIM2
#define TRIG_PWM_CHANNEL	TIM_OCMode_PWM1
#define TRIG_TIM_CHANNEL	TIM_Channel_3

   
/* Public functions ---------------------------------------------------------*/
void ultrasonic_init(void);
uint32_t ultrasonic_get_distance_cm(void);
uint32_t ultrasonic_get_distance_in(void);

/* Private functions ---------------------------------------------------------*/
static void _input_capture_config(void); // setup for echo pin
static void _output_compare_config(void); // setup for trig pin
