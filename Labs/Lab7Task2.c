/**
 * ECE4510 Lab7 Task 2
 *
 * @author      Tyler Thompson
 * @date        2/23/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private variables ---------------------------------------------------------*/
__IO uint32_t Value = 0;


/* Private functions ---------------------------------------------------------*/
static void ADC_Config(void);

/**
  * @brief  Handles the ADC internal interrupt
  * @param  None
  * @retval None
  */
void ADC_IRQHandler(void) {
  Value = ADC_GetConversionValue(ADC1);
  
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

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

  /* Enable ADC1 and GPIO clocks ****************************************/ 
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
