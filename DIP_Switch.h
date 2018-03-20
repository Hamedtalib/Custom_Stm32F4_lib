/**
 * Driver for DIP switch - 8 switches
 * Model: CTS 208-8
 *
 * @author      Tyler Thompson
 * @date        3/20/2018
 */

/* Includes ------------------------------------------------------------------*/

#define __DIP_SWITCH_H

#define DIP_GPIO_PORT	GPIOE
#define DIP_GPIO_CLK	RCC_AHB1Periph_GPIOE
#define DIP_GPIO_PINS	GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 
#define DIP_GPIO_ALIGN	((uint8_t) 0x0)


/* Private variables ---------------------------------------------------------*/

/* Public functions ---------------------------------------------------------*/
void dip_init(void);
uint8_t dip_get_switch_state(uint8_t switch_number);
uint8_t dip_get_all_switch_states(void);

/* Private functions ---------------------------------------------------------*/
