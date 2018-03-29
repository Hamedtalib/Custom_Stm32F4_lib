/**
 * Driver for serial communication between two devices
 *
 * @author      Tyler Thompson
 * @date        3/27/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "Serial_Com_Link.h"
#include "Timing_Delay.h"

__IO uint8_t scl_tx_index;
__IO char * scl_tx_buffer;
__IO uint8_t scl_tx_buffer_size = 16;

__IO uint8_t scl_rx_index;
__IO char * scl_rx_buffer;
__IO uint8_t scl_rx_buffer_size = 16;

/**
  * @brief  This function handles SPI interrupt request.
  * @param  None
  * @retval None
  */
void SCL_SPI_IRQHANDEL(void)
{
  /* SPI in Tramitter mode */
  if (SPI_I2S_GetITStatus(SCL_SPI, SPI_I2S_IT_TXE) == SET)
  { 
    if (scl_tx_index < scl_tx_buffer_size)
    {
      /* Send Transaction data */
      SPI_I2S_SendData(SCL_SPI, scl_tx_buffer[scl_tx_index++]);   
    }
    else
    {
      /* Disable the Tx buffer empty interrupt */
      SPI_I2S_ITConfig(SCL_SPI, SPI_I2S_IT_TXE, DISABLE);
    }
  }
  
  
  /* SPI in Receiver mode */
  if (SPI_I2S_GetITStatus(SCL_SPI, SPI_I2S_IT_RXNE) == SET)
  {
    if (scl_rx_index < scl_rx_buffer_size)
    {
      /* Receive Transaction data */
      scl_rx_buffer[scl_rx_index++] = SPI_I2S_ReceiveData(SCL_SPI);
    }
    else
    {
      /* Disable the Rx buffer not empty interrupt */
      SPI_I2S_ITConfig(SCL_SPI, SPI_I2S_IT_RXNE, DISABLE);
    }
  }
  
}


void scl_send_string(char *data) {
  scl_tx_index = 0;
  scl_tx_buffer = data;
  SPI_I2S_ITConfig(SCL_SPI, SPI_I2S_IT_TXE, ENABLE);
   /* Waiting the end of Data transfer */
  while (scl_tx_index < scl_tx_buffer_size){}
  delay(1); // 1 millisecond delay
}

void scl_send_char(char data) {
  scl_tx_index = 0;
  scl_tx_buffer[0] = data;
  SPI_I2S_ITConfig(SCL_SPI, SPI_I2S_IT_TXE, ENABLE);
   /* Waiting the end of Data transfer */
  while (scl_tx_index < 1){}
  delay(1); // 1 millisecond delay
}

char scl_receive_char(void) {
  scl_rx_index = 0;
  SPI_I2S_ITConfig(SCL_SPI, SPI_I2S_IT_RXNE, ENABLE);
  while (scl_rx_index < 1){}
  return scl_rx_buffer[0];
}

char *scl_receive_string(void) {
  scl_rx_index = 0;
  SPI_I2S_ITConfig(SCL_SPI, SPI_I2S_IT_RXNE, ENABLE);
  while (scl_rx_index < scl_rx_buffer_size){}
  return (char *)scl_rx_buffer;
}

void scl_init(uint8_t mode) {
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

  /* Peripheral Clock Enable -------------------------------------------------*/
  /* Enable the SPI clock */
  SCL_SPI_CLK_INIT(SCL_SPI_CLK, ENABLE);
  
  /* Enable GPIO clocks */
  RCC_AHB1PeriphClockCmd(SCL_GPIO_CLK, ENABLE);

  /* SPI GPIO Configuration --------------------------------------------------*/
  /* GPIO Deinitialisation */
  //GPIO_DeInit(SCL_GPIO_PORT);
  
  /* Connect SPI pins to AF5 */  
  GPIO_PinAFConfig(SCL_GPIO_PORT, SCL_SCK_SOURCE, SCL_SPI_AF);   
  GPIO_PinAFConfig(SCL_GPIO_PORT, SCL_MOSI_SOURCE, SCL_SPI_AF);
  GPIO_PinAFConfig(SCL_GPIO_PORT, SCL_MISO_SOURCE, SCL_SPI_AF);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;

  /* SPI SCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = SCL_SCK_PIN;
  GPIO_Init(SCL_GPIO_PORT, &GPIO_InitStructure);  

  /* SPI  MOSI pin configuration */
  GPIO_InitStructure.GPIO_Pin =  SCL_MOSI_PIN;
  GPIO_Init(SCL_GPIO_PORT, &GPIO_InitStructure);
  
  /* SPI  MISO pin configuration */
  GPIO_InitStructure.GPIO_Pin =  SCL_MISO_PIN;
  GPIO_Init(SCL_GPIO_PORT, &GPIO_InitStructure);
 
  /* SPI configuration -------------------------------------------------------*/
  SPI_I2S_DeInit(SCL_SPI);
  if(mode == 0) {
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  }
  else {
    SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
  }
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  
  SPI_Init(SCL_SPI, &SPI_InitStructure);
  
  /* Configure the Priority Group to 1 bit */                
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* Configure the SPI interrupt priority */
  NVIC_InitStructure.NVIC_IRQChannel = SCL_SPI_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable the SPI peripheral */
  SPI_Cmd(SCL_SPI, ENABLE);
  
  scl_tx_index = 0;
  scl_rx_index = 0;
  
  delay_init();
}

