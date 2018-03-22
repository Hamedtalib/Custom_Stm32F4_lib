/**
 * Driver for Ultra Sonic Sensor
 * Model: HC-SR4
 *
 * @author      Tyler Thompson
 * @date        3/20/2018
 */

/* Includes ------------------------------------------------------------------*/

#define __ULTRASONICSENSOR_H

#define ECHO_GPIO_PORT	GPIOA
#define ECHO_GPIO_PIN	GPIO_Pin_2
#define ECHO_GPIO_CLK	RCC_AHB1Periph_GPIOA
#define ECHO_TIM	TIM2
#define ECHO_TIM_CLK	RCC_APB1Periph_TIM2
#define ECHO_TIM_PIN	GPIO_PinSource2
#define ECHO_TIM_AF	GPIO_AF_TIM2
#define ECHO_TIM_CHANNEL	TIM_Channel_3	
#define ECHO_TIM_IRQ	TIM2_IRQn;

#define TRIG_GPIO_PORT	GPIOC
#define TRIG_GPIO_PIN	GPIO_Pin_6
#define TRIG_GPIO_CLK	RCC_AHB1Periph_GPIOC
#define TRIG_TIM	TIM3
#define TRIG_TIM_CLK	RCC_APB1Periph_TIM3
#define TRIG_TIM_PIN	GPIO_PinSource6
#define TRIG_TIM_AF	GPIO_AF_TIM3
#define TRIG_PWM_CHANNEL	TIM_OCMode_PWM2
#define TRIG_TIM_CHANNEL	TIM_Channel_1


/* Private variables ---------------------------------------------------------*/
//static __IO uint32_t echo_count = 0;
   
/* Public functions ---------------------------------------------------------*/
void ultrasonic_init(void);
void ultrasonic_trig(void);
void ultrasonic_listen(void);

/* Private functions ---------------------------------------------------------*/
static void _input_capture_config(void); // setup for echo pin
static void _output_compare_config(void); // setup for trig pin
