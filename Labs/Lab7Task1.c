/**
 * ECE4510 Lab7 Task 1
 *
 * @author      Tyler Thompson
 * @date        2/23/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Private variables ---------------------------------------------------------*/
uint16_t dataArray[1024] ;
uint16_t index = 0;


/* Private function prototypes -----------------------------------------------*/
static void TIM5_Config(void);
static void DAC_Ch2_Config(void);

/**
  * @brief  This function handles TIM5 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM5_IRQHandler(void)
{ 
  if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) 
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    if ( index == 1024 ) {
      index = 0;
    }
    
    DAC_SetChannel2Data(DAC_Align_12b_R, dataArray[index]);
    index = index + 1;
  }
}

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{

  /* fill the dataArray with data */
  for ( uint16_t i = 0; i < 1024; i = i + 1 ) {
    dataArray[i] = i * 4;
  }
  
 

  /* TIM6 Configuration ------------------------------------------------------*/
  TIM5_Config();  
  
  DAC_Ch2_Config();

  while (1);
}

/**             
  * @brief  TIM5 Configuration
  * @note   TIM5 configuration is based on APB1 frequency
  * @note   TIM5 Update event occurs each TIM5CLK/256   
  * @param  None
  * @retval None
  */
static void TIM5_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
  
  /* TIM5 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
  
  /* Time base configuration */

  TIM_TimeBaseStructure.TIM_Period = 3360;          
  TIM_TimeBaseStructure.TIM_Prescaler = 0;       
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;    
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

  
  /* TIM5 enable counter */
  TIM_Cmd(TIM5, ENABLE);
  
  TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
  TIM_SetCounter(TIM5, 0);
  TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
  
  /* Enable the TIM5 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  
}



/**
  * @brief  DAC Channel2 Configuration
  * @param  None
  * @retval None
  */
static void DAC_Ch2_Config(void)
{

  DAC_InitTypeDef  DAC_InitStructure;

  /* DAC channel2 Configuration */
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  DAC_Init(DAC_Channel_2, &DAC_InitStructure);
  
   /* Preconfiguration before using DAC----------------------------------------*/
  GPIO_InitTypeDef GPIO_InitStructure;


  /* GPIOA clock enable (to be used with DAC) */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);                         
  /* DAC Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

  /* DAC channel 2 (DAC_OUT2 = PA.5) configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

 
  /* Enable DAC Channel2 */
  DAC_Cmd(DAC_Channel_2, ENABLE);


}

