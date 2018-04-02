/**
 * Driver for temperature sensor
 * Model: LM32
 *
 * @author      Tyler Thompson
 * @date        4/2/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "Timing_Delay.h"


#define __TEMP_SENSOR_H

#define TEMP_GPIO_PORT	GPIOA
#define TEMP_GPIO_CLK	RCC_AHB1Periph_GPIOA
#define TEMP_GPIO_PIN   GPIO_Pin_4

#define TEMP_ADC	ADC1
#define TEMP_ADC_CHANNEL	ADC_Channel_4
#define	TEMP_ADC_CLK	RCC_APB1Periph_ADC1


/* Private variables ---------------------------------------------------------*/

/* Public functions ---------------------------------------------------------*/
void tempsensor_init(void);
uint32_t tempsensor_gettemp(void);

/* Private functions ---------------------------------------------------------*/
