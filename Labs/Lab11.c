#include "main.h"

//Initialization variables
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef   EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  DAC_InitTypeDef DAC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  ADC_InitTypeDef ADC_InitStructure;  
  TIM_TimeBaseInitTypeDef       TIM_TimeBaseStructure;
  TIM_ICInitTypeDef  TIM_ICInitStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  SPI_InitTypeDef       SPI_InitStructure;
  CAN_InitTypeDef       CAN_InitStructure;
  CAN_FilterInitTypeDef CAN_FilterInitStructure;

//Variables
/*
BS1 = 8
BS2 = 8
JSw = 4
CAN Frame = 20
CAN Prescalar = 4
500KBs * 4 * 20 = 40MBps
APB1Clk = 40MHz
HCLK = 40MHz * 4 = 160Mhz  
*/
uint8_t arrTrans[16];
uint8_t arrRec[16];
CanTxMsg trans;
CanRxMsg rec;

//Function headers

  
//Port Assignments
  /*
  Function      Module, Mode    Pin
  -------------------------------------   
  CAN1_Rx       AF              PB8
  CAN1_Tx       AF              PB9
  */

int main(void) {  
  
//Array initialization
  arrTrans[0] = 'A'; 
  arrTrans[1] = 'l'; 
  arrTrans[2] = 'p'; 
  arrTrans[3] = 'h'; 
  arrTrans[4] = 'a'; 
  arrTrans[5] = 'b'; 
  arrTrans[6] = 'e'; 
  arrTrans[7] = 't'; 
  arrTrans[8] = ' '; 
  arrTrans[9] = 'i'; 
  arrTrans[10] = 's'; 
  arrTrans[11] = ' '; 
  arrTrans[12] = 'l'; 
  arrTrans[13] = 'o'; 
  arrTrans[14] = 'n'; 
  arrTrans[15] = 'g'; 
    
//RCC Initializations
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  
//GPIO Initializations
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_CAN1); 
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_CAN1); 
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
//CAN Initializations
  CAN_OperatingModeRequest(CAN1, CAN_OperatingMode_Initialization);
  CAN_InitStructure.CAN_Prescaler = 4;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  CAN_InitStructure.CAN_SJW = CAN_SJW_4tq;
  CAN_InitStructure.CAN_BS1 = CAN_BS1_8tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = DISABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  CAN_Init(CAN1, &CAN_InitStructure);
  
//CAN Filter Initialization
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000; //Is 0 or 1 a dont care
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
  CAN_FilterInitStructure.CAN_FilterNumber = 0;
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);
 
//CAN Interrupt Initializations 
  NVIC_InitStructure.NVIC_IRQChannel = CAN1_TX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  CAN_ITConfig(CAN1, CAN_IT_TME | CAN_IT_FMP0, ENABLE);
  
//CanTxMsg initialization
  trans.StdId = 0x11;
  trans.IDE = CAN_Id_Standard;
  trans.RTR = CAN_RTR_Data;
  trans.DLC = 1;
  trans.Data[0] = arrTrans[0];
  
//CanRxMsg Initialization
  rec.StdId = 0x00;
  rec.IDE = CAN_Id_Standard;
  rec.RTR = CAN_RTR_Data;
  rec.DLC = 1;
  rec.FMI = 0;
  
  //Initialize to empty
  for(int i = 0; i < 16; i++) {
    rec.Data[i] = 0x00;
  }
  
//Startups
  CAN_OperatingModeRequest(CAN1, CAN_OperatingMode_Normal);
  CAN_Transmit(CAN1, &trans);
  
  while(1);
  
}


