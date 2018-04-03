/**
 * Driver for CAN, simple communication
 *
 * @author      Tyler Thompson
 * @date        4/3/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "Timing_Delay.h"


#define __CAN_SIMPLE_H

#define CANS_RXIRQ_CHANNEL	CAN1_RX0_IRQn
#define CANS_TXIRQ_CHANNEL	CAN1_TX_IRQn
#define CANS_RX_IRQHANDLE	CAN1_RX0_IRQHandler

#define CANS_CAN	CAN1
#define CANS_CLK	RCC_APB1Periph_CAN1
#define CANS_GPIO_CLK	RCC_AHB1Periph_GPIOA
#define CANS_GPIO_PORT	GPIOA
#define CANS_GPIO_RX_PIN	GPIO_Pin_11
#define CANS_GPIO_TX_PIN	GPIO_Pin_12
#define CANS_GPIO_RX_SOURCE	GPIO_PinSource11
#define CANS_GPIO_TX_SOURCE	GPIO_PinSource12
#define CANS_AF_PORT		GPIO_AF_CAN1


/* Private variables ---------------------------------------------------------*/

/* Public functions ---------------------------------------------------------*/
void cans_init(void);
uint8_t * cans_get_message(void);
void cans_send_message(uint8_t *send_data);
/* Private functions ---------------------------------------------------------*/
