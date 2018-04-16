/**
 * Driver for DIP switch - 8 switches
 * Model: CTS 208-8
 *
 * @author      Tyler Thompson
 * @date        3/20/2018
 */

/* Includes ------------------------------------------------------------------*/

#define __DIP_SWITCH_H

#define DIP_GPIO_PORT	GPIOA
#define DIP_GPIO_CLK	RCC_AHB1Periph_GPIOA
//#define DIP_GPIO_PINS	DIP_GPIO_PIN0 | DIP_GPIO_PIN1 | DIP_GPIO_PIN2 | DIP_GPIO_PIN3 | DIP_GPIO_PIN4 | DIP_GPIO_PIN5 | DIP_GPIO_PIN6 | DIP_GPIO_PIN7 
#define DIP_GPIO_PINS	DIP_GPIO_PIN0 | DIP_GPIO_PIN1  
#define DIP_GPIO_ALIGN	((uint8_t) 0x8)
#define DIP_GPIO_PIN0   GPIO_Pin_7
#define DIP_GPIO_PIN1   GPIO_Pin_6
#define DIP_GPIO_PIN2   GPIO_Pin_9
#define DIP_GPIO_PIN3   GPIO_Pin_10
#define DIP_GPIO_PIN4   GPIO_Pin_11
#define DIP_GPIO_PIN5   GPIO_Pin_12
#define DIP_GPIO_PIN6   GPIO_Pin_13
#define DIP_GPIO_PIN7   GPIO_Pin_14


/* Private variables ---------------------------------------------------------*/

/* Public functions ---------------------------------------------------------*/
void dip_init(void);
uint8_t dip_get_switch_state(uint8_t switch_number);
uint8_t dip_get_all_switch_states(void);

/* Private functions ---------------------------------------------------------*/
