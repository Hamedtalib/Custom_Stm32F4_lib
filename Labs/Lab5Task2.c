/**
 * ECE4510 Lab5 Task 2
 *
 * @author      Tyler Thompson
 * @date        2/9/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"


/* Private variables ---------------------------------------------------------*/
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
uint16_t CCR1_Val = 0;
uint16_t TimerPeriod = 0;
uint16_t Frequency = 0;

/* Private function prototypes -----------------------------------------------*/
void TIM_Config(void);
void GPIO_PC6_Config(void);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

  //output frequency is 7.5KHz
  Frequency = 7500;
  
  /*PC6 Configuration */
  GPIO_PC6_Config();
  
  /* TIM Configuration */
  TIM_Config();
  
  while (1);
}

/**
  * @brief  Configure the TIM3 Ouput Channels.
  * @param  None
  * @retval None
  */
void TIM_Config(void)
{
  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  
  /* Compute the value to be set in ARR regiter to generate signal frequency at 7.5 Khz */
  TimerPeriod = (SystemCoreClock / (Frequency * 2) ) - 1;
  //40% duty cycle
  CCR1_Val = (uint16_t) (((uint32_t) 4 * (TimerPeriod - 1)) / 10);
  
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIM3, &TIM_OCInitStructure);

  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);
}

void GPIO_PC6_Config(void) {
  GPIO_InitTypeDef GPIO_InitStructure;

  
  /* GPIOC clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  
  /* GPIOC Configuration: TIM3 CH1 (PC6) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOC, &GPIO_InitStructure); 

  /* Connect TIM3 pins to AF2 */  
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
}