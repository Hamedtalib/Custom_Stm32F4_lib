/**
 * Driver for CAN, simple comunication
 *
 * @author      Tyler Thompson
 * @date        4/3/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "CAN_Simple.h"

CanRxMsg RxMessage;
CanTxMsg TxMessage;

/**
  * @brief  This function handles CAN RX request.
  * @param  None
  * @retval None
  */
void CANS_RX_IRQHANDLE(void)
{
  CAN_Receive(CANS_CAN, CAN_FIFO0, &RxMessage);
}


uint8_t * cans_get_message(void) {
	return RxMessage.Data;
}

void cans_send_message(uint8_t *send_data) {
	//TxMessage.Data = send_data;
}

/**
  * @brief  Initialize the CAN and GPIO port
  * @param  None
  * @retval None
  */
void cans_init(void) {
  NVIC_InitTypeDef  NVIC_InitStructure;
  GPIO_InitTypeDef  GPIO_InitStructure;
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;

  /* NVIC interrupt initialization for RX */
  NVIC_InitStructure.NVIC_IRQChannel = CANS_RXIRQ_CHANNEL;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  
  /* CAN GPIOs configuration **************************************************/

  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(CANS_GPIO_CLK, ENABLE);

  /* Connect CAN pins to AF9 */
  GPIO_PinAFConfig(CANS_GPIO_PORT, CANS_GPIO_RX_SOURCE, CANS_AF_PORT);
  GPIO_PinAFConfig(CANS_GPIO_PORT, CANS_GPIO_TX_SOURCE, CANS_AF_PORT); 
  
  /* Configure CAN RX and TX pins */
  GPIO_InitStructure.GPIO_Pin = CANS_GPIO_RX_PIN | CANS_GPIO_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(CANS_GPIO_PORT, &GPIO_InitStructure);

  /* CAN configuration ********************************************************/  
  /* Enable CAN clock */
  RCC_APB1PeriphClockCmd(CANS_CLK, ENABLE);
  
  /* CAN register init */
  CAN_DeInit(CANS_CAN);

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = DISABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    
  /* CAN Baudrate = 500Kbs */
  CAN_InitStructure.CAN_BS1 = CAN_BS1_14tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_6tq;
  CAN_InitStructure.CAN_Prescaler = 4;
  CAN_Init(CANS_CAN, &CAN_InitStructure);

  /* CAN filter init */
  CAN_FilterInitStructure.CAN_FilterNumber = 0;
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);
  
  /* Transmit Structure preparation */
  TxMessage.StdId = 0x321;
  TxMessage.ExtId = 0x01;
  TxMessage.RTR = CAN_RTR_DATA;
  TxMessage.IDE = CAN_ID_STD;
  TxMessage.DLC = 1;

  /* receive structure preparation */
   uint8_t ubCounter = 0;

  RxMessage.StdId = 0x00;
  RxMessage.ExtId = 0x00;
  RxMessage.IDE = CAN_ID_STD;
  RxMessage.DLC = 0;
  RxMessage.FMI = 0;
  for (ubCounter = 0; ubCounter < 8; ubCounter++)
  {
    RxMessage.Data[ubCounter] = 0x00;
  }
  
  /* Enable FIFO 0 message pending Interrupt */
  CAN_ITConfig(CANS_CAN, CAN_IT_FMP0, ENABLE);
}


