/**
 * Driver for temperature sensor
 * Model: LM32
 *
 * @author      Tyler Thompson
 * @date        4/2/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "Temp_Sensor.h"


float tempsensor_gettemp(void) {
  float temperature;
 /* Start ADC Software Conversion */ 
  ADC_SoftwareStartConv(TEMP_ADC);
  /* wait for conversion to finish */
 while(!ADC_IT_EOC){}
  /* get the temperature in celcius */
  temperature = (ADC_GetConversionValue(TEMP_ADC) / 27.4);
  return temperature;
}

/**
  * @brief  Initialize the ADC and GPIO port
  * @param  None
  * @retval None
  */
void tempsensor_init(void) {
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  GPIO_InitTypeDef      GPIO_InitStructure;
 // NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable ADC and GPIO clocks ****************************************/ 
  RCC_AHB1PeriphClockCmd(TEMP_GPIO_CLK, ENABLE);  
  RCC_APB2PeriphClockCmd(TEMP_ADC_CLK, ENABLE);

  /* Configure ADC Channel pin as analog input ******************************/
  GPIO_InitStructure.GPIO_Pin = TEMP_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(TEMP_GPIO_PORT, &GPIO_InitStructure);


  /* ADC Common Init **********************************************************/
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);


  /* ADC Init ****************************************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(TEMP_ADC, &ADC_InitStructure);


  /* ADC regular channel configuration *************************************/
  ADC_RegularChannelConfig(TEMP_ADC, TEMP_ADC_CHANNEL, 1, ADC_SampleTime_3Cycles);

  ADC_ITConfig(TEMP_ADC,ADC_IT_EOC,ENABLE);

  ADC_EOCOnEachRegularChannelCmd(TEMP_ADC, ENABLE);
  
  /* Enable ADC */
  ADC_Cmd(TEMP_ADC, ENABLE);

  
}

