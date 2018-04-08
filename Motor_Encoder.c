/** 
 * Driver for motor encoder
 *
 * @author      Tyler Thompson
 * @date        4/5/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "Motor_Encoder.h"

/* Private variables ---------------------------------------------------------*/
static __IO uint16_t uhIC3ReadValue1 = 0;
static __IO uint16_t uhIC3ReadValue2 = 0;
static __IO uint16_t uhIC3ReadValue3 = 0;
static __IO uint16_t uhCaptureNumber = 0;
static __IO uint32_t uwCapture1 = 0;
static __IO uint32_t uwCapture2 = 0;
static __IO uint32_t encoder_frequency = 0;
//__IO uint32_t uwTIM2Duty = 0;

/**
  * @brief  This function handles TIM global interrupt request.
  * @param  None
  * @retval None
  */
void ENCODER_TIM_IRQ_HANDLE(void)
{ 
  if(TIM_GetITStatus(ENCODER_TIM, ENCODER_TIM_IT) == SET) 
  {
    /* Clear TIM Capture compare interrupt pending bit */
    TIM_ClearITPendingBit(ENCODER_TIM, ENCODER_TIM_IT);
    if(uhCaptureNumber == 0)
    {
      /* Get the Input Capture value */
      uhIC3ReadValue1 = TIM_GetCapture1(ENCODER_TIM);
      uhCaptureNumber = 1;
    }
    else if(uhCaptureNumber == 1)
    {
      /* Get the Input Capture value */
      uhIC3ReadValue2 = TIM_GetCapture1(ENCODER_TIM); 
      
      /* Capture computation */
     if (uhIC3ReadValue2 > uhIC3ReadValue1)
     {
        uwCapture1 = (uhIC3ReadValue2 - uhIC3ReadValue1);
     }
     else if (uhIC3ReadValue2 < uhIC3ReadValue1)
     {
        uwCapture1 = ((0xFFFF - uhIC3ReadValue1) + uhIC3ReadValue2);
     }
     else
     {
        uwCapture1 = 0;
     }
    }  
   else if (uhCaptureNumber == 2) {
       /* Get the Input Capture value */
      uhIC3ReadValue3 = TIM_GetCapture3(TIM2);

       /* Capture computation */
      if (uhIC3ReadValue3 > uhIC3ReadValue2)
      {
        uwCapture2 = (uhIC3ReadValue3 - uhIC3ReadValue2);
      }
      else if (uhIC3ReadValue3 < uhIC3ReadValue2)
      {
        uwCapture2 = ((0xFFFF - uhIC3ReadValue2) + uhIC3ReadValue3);
      }
      else
      {
        uwCapture2 = 0;
      }

      /* Frequency computation */ 
      encoder_frequency = (((uint32_t) SystemCoreClock / (uwCapture2 + uwCapture1)) / 2);
//      uwTIM2Duty = ((uwCapture2 * 100) / (uwCapture2 + uwCapture1));
      uhCaptureNumber = 0;
     
    }
 
  }
}


/**
  * @brief  Configure the TIM and GPIO port for the motor encoder
  * @param  None
  * @retval None
  */
void encoder_init(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_ICInitTypeDef  TIM_ICInitStructure;

  /* TIM clock enable */
  RCC_APB1PeriphClockCmd(ENCODER_TIM_CLK, ENABLE);

  /* GPIO clock enable */
  RCC_AHB1PeriphClockCmd(ENCODER_GPIO_CLK, ENABLE);

  /* TIM channel configuration */
  GPIO_InitStructure.GPIO_Pin =  ENCODER_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(ENCODER_GPIO_PORT, &GPIO_InitStructure);

  /* Connect TIM pins to AF1 */
  GPIO_PinAFConfig(ENCODER_GPIO_PORT, ENCODER_TIM_PIN, ENCODER_TIM_AF);

  /* Enable the TIM global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = ENCODER_TIM_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

/* TIM configuration: Input Capture mode ---------------------*/
  TIM_ICInitStructure.TIM_Channel = ENCODER_TIM_CHANNEL;
  TIM_ICInitStructure.TIM_ICPolarity =  TIM_ICPolarity_BothEdge;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;

  TIM_ICInit(ENCODER_TIM, &TIM_ICInitStructure);

  /* TIM enable counter */
  TIM_Cmd(ENCODER_TIM, ENABLE);

  /* Enable the CC Interrupt Request */
  TIM_ITConfig(ENCODER_TIM, ENCODER_TIM_IT, ENABLE);

}

uint32_t encoder_get_frequency(void) {
	return encoder_frequency;
}
