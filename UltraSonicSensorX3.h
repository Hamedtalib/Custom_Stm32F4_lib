/**
 * Driver for Ultra Sonic Sensor
 ; Handles 3 sensors
 * Model: HC-SR4
 *
 * @author      Tyler Thompson
 * @date        3/23/2018
 */

/* Includes ------------------------------------------------------------------*/

#define __ULTRASONICSENSOR_H

/* Main Echo variables */
#define ECHO_GPIO_PORT	GPIOA
#define ECHO_GPIO_CLK	RCC_AHB1Periph_GPIOA
#define ECHO_TIM_CLK	RCC_APB1Periph_TIM5
#define ECHO_TIM_AF	GPIO_AF_TIM5
#define ECHO_TIM_IRQ	TIM5_IRQn
#define ECHO_TIM	TIM5
#define ECHO_GPIO_PINS	ECHO1_GPIO_PIN | ECHO2_GPIO_PIN | ECHO3_GPIO_PIN
#define ECHO_TIM_PINS	ECHO1_TIM_PIN | ECHO2_TIM_PIN | ECHO3_TIM_PIN
#define ECHO_TIM_CHANNELS	ECHO1_TIM_CHANNEL | ECHO2_TIM_CHANNEL | ECHO3_TIM_CHANNEL
/* variables for sensor 1 */
#define ECHO1_GPIO_PIN	GPIO_Pin_0
#define ECHO1_TIM_PIN	GPIO_PinSource0
#define ECHO1_TIM_CHANNEL	TIM_Channel_1
#define ECHO1_TIM_IT     TIM_IT_CC1
/* variables for sensor 2 */
#define ECHO2_GPIO_PIN	GPIO_Pin_1
#define ECHO2_TIM_PIN	GPIO_PinSource1
#define ECHO2_TIM_CHANNEL	TIM_Channel_2
#define ECHO2_TIM_IT     TIM_IT_CC2
/* variables for sensor 3 */
#define ECHO3_GPIO_PIN	GPIO_Pin_2
#define ECHO3_TIM_PIN	GPIO_PinSource2
#define ECHO3_TIM_CHANNEL	TIM_Channel_3
#define ECHO3_TIM_IT     TIM_IT_CC3

/* Main trigger variables, all 3 sensors use the same pin for trigger*/
#define TRIG_GPIO_PORT	GPIOB
#define TRIG_GPIO_CLK	RCC_AHB1Periph_GPIOB
#define TRIG_TIM_CLK	RCC_APB1Periph_TIM2
#define TRIG_TIM_AF	GPIO_AF_TIM2
#define TRIG_PWM_CHANNEL	TIM_OCMode_PWM1
#define TRIG_TIM	TIM2
#define TRIG_GPIO_PIN	GPIO_Pin_10
#define TRIG_TIM_PIN	GPIO_PinSource10


/* Public functions ---------------------------------------------------------*/
void ultrasonic_init(void);
uint32_t ultrasonic1_get_distance_cm(void);
uint32_t ultrasonic2_get_distance_cm(void);
uint32_t ultrasonic3_get_distance_cm(void);

/* Private functions ---------------------------------------------------------*/
static void _input_capture_config(void); // setup for echo pin
static void _output_compare_config(void); // setup for trig pin
