/**
 * Driver for DIP switch - 8 switches
 * Model: CTS 208-8
 *
 * @author      Tyler Thompson
 * @date        3/20/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "DIP_Switch.h"


/**
  * @brief  Initialize the switches
  * @param  None
  * @retval None
  */
void dip_init(void) {
  GPIO_InitTypeDef GPIO_InitStructure;

  /* GPIO clock enable */
  RCC_AHB1PeriphClockCmd(DIP_GPIO_CLK, ENABLE);

  /* GPIO Configuration */
  GPIO_InitStructure.GPIO_Pin = DIP_GPIO_PINS;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;

  GPIO_Init(DIP_GPIO_PORT, &GPIO_InitStructure); 
}

/**
  * @brief  Get the state of a specified switch
  * @param  None
  * @retval None
  */
uint8_t dip_get_switch_state(uint8_t switch_number) {
  uint16_t port_number = GPIO_Pin_0;
  switch(switch_number) {
    case 0 :
      port_number = GPIO_Pin_0;
      break;
    case 1 :
      port_number = GPIO_Pin_1;
      break;
    case 2 :
      port_number = GPIO_Pin_2;
      break;
    case 3 :
      port_number = GPIO_Pin_3;
      break;
    case 4 :
      port_number = GPIO_Pin_4;
      break;
    case 5 :
      port_number = GPIO_Pin_5;
      break;
    case 6 :
      port_number = GPIO_Pin_6;
      break;
    case 7 :
      port_number = GPIO_Pin_7;
      break;
    case 8 :
      port_number = GPIO_Pin_8;
      break;
    default:
      port_number = GPIO_Pin_0;
  }
  return GPIO_ReadInputDataBit(DIP_GPIO_PORT, port_number);
}

/**
  * @brief  Get the state of all switches
  * @param  None
  * @retval None
  */
uint8_t dip_get_all_switch_states(void) {
  return ((uint8_t)(DIP_GPIO_PORT->IDR >> DIP_GPIO_ALIGN));
}
