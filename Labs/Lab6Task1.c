/**
 * ECE4510 Lab6 Task 1
 *
 * @author      Tyler Thompson
 * @date        2/19/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"


/* Private variables ---------------------------------------------------------*/

GPIO_InitTypeDef GPIO_InitStructure;


static uint16_t counter = 0x00FF;

/* Private function prototypes -----------------------------------------------*/
void TIM2_Config(void);
void GPIO_PC7_15_Config(void);



/**
  * @brief  This function handles TIM2 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{ 
  if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 
  {
    /* Clear TIM2 Capture compare interrupt pending bit */
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
      //set PC7 to high
      GPIOC->BSRRL = GPIO_Pin_7;
      //decrement the couter
      counter = counter -1;
      
      //output the couter to the data pins
      GPIOC->ODR = counter << 8;
      
      //set PC7 to low
      GPIOC->BSRRH = GPIO_Pin_7;
      
      //reset the couter
      if ( counter == 0x0 ) {
        
        counter = 0x00FF;
      }
  }
}


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

 
  /*PC7_15 Configuration */
  GPIO_PC7_15_Config();
  
  //start with all pins set to high
  GPIOC->ODR = 0xFFFF;
  
  //TIM2 configuration
  TIM2_Config();
 
  while (1);
}

/**
  * @brief  Configure the TIM2 Ouput Channels.
  * @param  None
  * @retval None
  */
void TIM2_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  
  
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 2099;
  TIM_TimeBaseStructure.TIM_Prescaler = 9;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
 
  
  TIM_Cmd(TIM2, ENABLE);
  
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  TIM_SetCounter(TIM2, 0);
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

   /* Enable the TIM2 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);


}


void GPIO_PC7_15_Config(void) {
    
  /* GPIOC clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  
  /* GPIOC Configuration: (PC 7 - 15) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOC, &GPIO_InitStructure); 

}