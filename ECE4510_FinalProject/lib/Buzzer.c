/**
 * Driver for buzzer
 *
 * @author      Tyler Thompson
 * @date        4/2/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "Buzzer.h"


void buz_sound_delay(uint32_t frequency, uint32_t delay_time) {
  buz_start_sound(frequency);
  delay(delay_time);
  buz_stop_sound();
}

void buz_start_sound(uint32_t frequency) {
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;

    /* Compute the value to be set in ARR regiter to generate signal frequency */
  uint32_t TimerPeriod = (SystemCoreClock / (frequency * 2) -1);
  
  //duty cycle 50%
  uint16_t CCR1_Val = (uint16_t) (((uint32_t) 50 * (TimerPeriod + 1)) / 100);
  
  TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(BUZ_TIM, &TIM_TimeBaseStructure);
  
    /* PWM1 Mode configuration */
  TIM_OCInitStructure.TIM_OCMode = BUZ_PWM_CHANNEL;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC3Init(BUZ_TIM, &TIM_OCInitStructure);
  
  TIM_Cmd(BUZ_TIM, ENABLE);
}

void buz_stop_sound(void) {
   TIM_Cmd(BUZ_TIM, DISABLE);
}

/**
  * @brief  Initialize the TIM and GPIO port
  * @param  None
  * @retval None
  */
void buz_init(void) {
  GPIO_InitTypeDef GPIO_InitStructure;

  /* GPIO clock enable */
  RCC_AHB1PeriphClockCmd(BUZ_GPIO_CLK, ENABLE);

  /* GPIO Configuration */
  GPIO_InitStructure.GPIO_Pin = BUZ_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;

  GPIO_Init(BUZ_GPIO_PORT, &GPIO_InitStructure); 

  /* Connect TIM pins to AF2 */  
  GPIO_PinAFConfig(BUZ_GPIO_PORT, BUZ_GPIO_SOURCE, BUZ_TIM_AF);
  
/* TIM clock enable */
  RCC_APB1PeriphClockCmd(BUZ_TIM_CLK, ENABLE);

  delay_init();
}

