/**
 * Driver for LED Panel - 8 LEDs
 * Model: KingBright DC-10EWA
 *
 * @author      Tyler Thompson
 * @date        3/26/2018
 * @note	LED panel has 10 LEDs, but this driver only uses 8
 */

/* Includes ------------------------------------------------------------------*/

#define __LED_PANEL_X8_H

#define LED_GPIO_PORT	GPIOC
#define LED_GPIO_CLK	RCC_AHB1Periph_GPIOC
#define LED_GPIO_PINS	LED_GPIO_PIN0 | LED_GPIO_PIN1 | LED_GPIO_PIN2 | LED_GPIO_PIN3 | LED_GPIO_PIN4 | LED_GPIO_PIN5 | LED_GPIO_PIN6 | LED_GPIO_PIN7 
#define LED_GPIO_ALIGN	((uint8_t) 0x2)
#define LED_GPIO_PIN0   GPIO_Pin_2
#define LED_GPIO_PIN1   GPIO_Pin_3
#define LED_GPIO_PIN2   GPIO_Pin_4
#define LED_GPIO_PIN3   GPIO_Pin_5
#define LED_GPIO_PIN4   GPIO_Pin_6
#define LED_GPIO_PIN5   GPIO_Pin_7
#define LED_GPIO_PIN6   GPIO_Pin_8
#define LED_GPIO_PIN7   GPIO_Pin_9


/* Private variables ---------------------------------------------------------*/

/* Public functions ---------------------------------------------------------*/
void led_init(void);
void led_set_state(uint8_t led_number, uint8_t state);
void led_set_all(uint16_t states);

/* Private functions ---------------------------------------------------------*/
