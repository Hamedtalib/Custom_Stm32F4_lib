/**
 * ECE4510 Lab7 Task 3
 *
 * @author      Tyler Thompson
 * @date        2/23/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private variables ---------------------------------------------------------*/
__IO uint32_t Value = 0;
__IO uint32_t NewValue = 0;


/* Private functions ---------------------------------------------------------*/
static void ADC_Config(void);
static void TIM5_Config(void);
static void DAC_Ch2_Config(void);

/**
  * @brief  Handles the ADC internal interrupt
  * @param  None
  * @retval None
  */
void ADC_IRQHandler(void) {
  Value = ADC_GetConversionValue(ADC1);
  
}

/**
  * @brief  This function handles TIM5 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM5_IRQHandler(void)
{ 
  if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) 
  {
    /* clear iterrupt bit */
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    
    /* calculate value to output to DAC based on ADC input */
    NewValue = 0.65 * Value + 800;
    DAC_SetChannel2Data(DAC_Align_12b_R, NewValue);
    
  }
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

  /* TIM5 config */
  TIM5_Config();  
  
  /* DAC config */
  DAC_Ch2_Config();

  /* ADC configuration */
  ADC_Config();

  /* Start ADC Software Conversion */ 
  ADC_SoftwareStartConv(ADC1);

  while (1);
}

/**
  * @brief  ADC1 channel9 
  * @note   This function Configure the ADC peripheral  
            1) Enable peripheral clocks
            3) Configure ADC Channel9 pin as analog input
            4) Configure ADC1 Channel9 
  * @param  None
  * @retval None
  */
static void ADC_Config(void)
{
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  GPIO_InitTypeDef      GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable ADCx and GPIO clocks ****************************************/ 
 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  

  /* Configure ADC1 Channel9 pin as analog input on PB1 ******************************/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  /* ADC Common Init **********************************************************/
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);

  /* ADC3 Init ****************************************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel9 configuration *************************************/
  ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_3Cycles);

  ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
  
 
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
