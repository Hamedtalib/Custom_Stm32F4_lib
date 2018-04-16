/**
 * Driver for LED Panel - 8 LEDs
 * Model: KingBright DC-10EWA
 *
 * @author      Tyler Thompson
 * @date        3/26/2018
 * @note	LED panel has 10 LEDs, but this driver only uses 8
 */

/* Includes ------------------------------------------------------------------*/
#include "LED_Panel_X8.h"


/**
  * @brief  Initialize the LEDS
  * @param  None
  * @retval None
  */
void led_init(void) {
  GPIO_InitTypeDef GPIO_InitStructure;

  /* GPIO clock enable */
  RCC_AHB1PeriphClockCmd(LED_GPIO_CLK, ENABLE);

  /* GPIO Configuration */
  GPIO_InitStructure.GPIO_Pin = LED_GPIO_PINS;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;

  GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure); 
  
  led_set_all(0xff); // initalize all leds off
  delay_init(); // initialize delays for blink
}

/**
  * @brief  Set the state of an LED
  * @param  led_number - the number of the LED to work with
  * @param  state - the state to put the LED in
  * @retval None
  */
void led_set_state(uint8_t led_number, uint8_t state) {
  uint16_t port_number = LED_GPIO_PIN0;
  switch(led_number) {
    case 0 :
      port_number = LED_GPIO_PIN0;
      break;
    case 1 :
      port_number = LED_GPIO_PIN1;
      break;
    case 2 :
      port_number = LED_GPIO_PIN2;
      break;
    case 3 :
      port_number = LED_GPIO_PIN3;
      break;
    case 4 :
      port_number = LED_GPIO_PIN4;
      break;
    case 5 :
      port_number = LED_GPIO_PIN5;
      break;
    case 6 :
      port_number = LED_GPIO_PIN6;
      break;
    case 7 :
      port_number = LED_GPIO_PIN7;
      break;
    default:
      port_number = LED_GPIO_PIN0;
  }
  if ( state == 0 ) {
	GPIO_SetBits(LED_GPIO_PORT, port_number);
  }
  else {
	GPIO_ResetBits(LED_GPIO_PORT, port_number);
  }
}

/**
  * @brief  Set the state of all LEDs
  * @param  states - The states of a LEDs to set
  * @retval None
  */
void led_set_all(uint16_t states) {
   LED_GPIO_PORT->ODR = (states << LED_GPIO_ALIGN);
}

/**
  * @brief  Blink and LED a specified number of times with a specified delay
  * @param  led_number - Which LED to work with
  * @param  times - Number of times to blink
  * @param  delay - number of milliseconds to delay between blinks
  * @retval None
  */
void led_blink(uint8_t led_number, uint32_t times, uint32_t delay_time) {
	for(uint8_t i = 0; i < times; i++) {
		led_set_state(led_number, 1);
		delay(delay_time / 2);
		led_set_state(led_number, 0);
		delay(delay_time / 2);

	}
}
