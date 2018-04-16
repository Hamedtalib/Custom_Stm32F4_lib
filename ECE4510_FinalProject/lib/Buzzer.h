/**
 * Driver for buzzer
 *
 * @author      Tyler Thompson
 * @date        4/2/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "Timing_Delay.h"

#define __BUZZER_H

#define BUZ_GPIO_PORT	GPIOB
#define BUZ_GPIO_CLK	RCC_AHB1Periph_GPIOB
#define BUZ_GPIO_PIN    GPIO_Pin_0
#define BUZ_GPIO_SOURCE	GPIO_PinSource0

#define BUZ_TIM		TIM3
#define BUZ_TIM_CLK	RCC_APB1Periph_TIM3
#define BUZ_TIM_AF	GPIO_AF_TIM3
#define BUZ_PWM_CHANNEL	TIM_OCMode_PWM1
#define BUZ_TIM_CHANNEL	TIM_Channel_3

/* Private variables ---------------------------------------------------------*/

/* Public functions ---------------------------------------------------------*/
void buz_init(void);
void buz_sound_delay(uint32_t frequency, uint32_t delay_time);
void buz_start_sound(uint32_t frequency);
void buz_stop_sound(void);

/* Private functions ---------------------------------------------------------*/
