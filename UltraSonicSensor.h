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

#define ECHO_GPIO_PORT	GPIOB
#define ECHO_GPIO_PIN	GPIO_Pin_6
#define ECHO_GPIO_CLK	RCC_AHB1Periph_GPIOB
#define ECHO_TIM	TIM4
#define ECHO_TIM_CLK	RCC_APB1Periph_TIM4
#define ECHO_TIM_PIN	GPIO_PinSource6
#define ECHO_TIM_AF	GPIO_AF_TIM4
#define ECHO_TIM_CHANNEL	TIM_Channel_1	
#define ECHO_TIM_IRQ	TIM4_IRQn;

#define TRIG_GPIO_PORT	GPIOA
#define TRIG_GPIO_PIN	GPIO_Pin_0
#define TRIG_GPIO_CLK	RCC_AHB1Periph_GPIOA
#define TRIG_TIM	TIM5
#define TRIG_TIM_CLK	RCC_APB1Periph_TIM5
#define TRIG_TIM_PIN	GPIO_PinSource0
#define TRIG_TIM_AF	GPIO_AF_TIM5
#define TRIG_PWM_CHANNEL	TIM_OCMode_PWM1
#define TRIG_TIM_CHANNEL	TIM_Channel_1
#define TRIG_TIM_IRQ	TIM5_IRQn;


/* Private variables ---------------------------------------------------------*/
//static __IO uint32_t echo_count = 0;
   
/* Public functions ---------------------------------------------------------*/
void ultrasonic_init(void);
void ultrasonic_trig(void);
void ultrasonic_listen(void);
uint32_t get_distance_cm(void);
uint32_t get_distance_in(void);

/* Private functions ---------------------------------------------------------*/
static void _input_capture_config(void); // setup for echo pin
static void _output_compare_config(void); // setup for trig pin
