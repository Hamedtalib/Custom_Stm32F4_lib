/** 
 * Driver for PWM Motor
 *
 * @author      Tyler Thompson
 * @date        4/5/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "PWM_Motor_Control.h"

/**
  * @brief  Initialize the PWM motor
  * @param  None
  * @retval None
  */
void pwmmotor_init(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;

  /* GPIO clock enable */
  RCC_AHB1PeriphClockCmd(MOTOR_GPIO_CLK, ENABLE);

  /* GPIO Configuration */
  GPIO_InitStructure.GPIO_Pin = MOTOR_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;

  GPIO_Init(MOTOR_GPIO_PORT, &GPIO_InitStructure); 

  /* Connect TIM pins to AF2 */  
  GPIO_PinAFConfig(MOTOR_GPIO_PORT, MOTOR_TIM_PIN, MOTOR_TIM_AF);
/* TIM clock enable */
  RCC_APB1PeriphClockCmd(MOTOR_TIM_CLK, ENABLE);

  /* Compute the value to be set in ARR regiter to generate signal frequency */
  uint32_t TimerPeriod = (SystemCoreClock / (frequency * 2)) - 1;
  
  //duty cycle
  uint16_t CCR1_Val = (uint16_t) (((uint32_t) duty * (TimerPeriod - 1)) / 10);

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(MOTOR_TIM, &TIM_TimeBaseStructure); 

  /* PWM1 Mode configuration */
  TIM_OCInitStructure.TIM_OCMode = MOTOR_PWM_CHANNEL;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC3Init(MOTOR_TIM, &TIM_OCInitStructure);
  
   /* TIM enable counter */
  TIM_Cmd(MOTOR_TIM, ENABLE);

}

/**
  * @brief  Set the duty cycle for the motor
  * @param  duty - The duty cycle to set
  * @retval None
  */
void pwmmotor_set_duty(uint32_t duty) {
  TIM_OCInitTypeDef  TIM_OCInitStructure;

  uint32_t TimerPeriod = (SystemCoreClock / (frequency * 2)) - 1;
  
  //duty cycle
  uint16_t CCR1_Val = (uint16_t) (((uint32_t) duty * (TimerPeriod - 1)) / 10);

  /* PWM1 Mode configuration */
  TIM_OCInitStructure.TIM_OCMode = MOTOR_PWM_CHANNEL;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC3Init(MOTOR_TIM, &TIM_OCInitStructure);
}

/**
  * @brief  Enable the motor
  * @param  None
  * @retval None
  */
void pwmmotor_enable(void) {
  TIM_Cmd(MOTOR_TIM, ENABLE);
}

/**
  * @brief  Disable to motor
  * @param  None
  * @retval None
  */
void pwmmotor_diable(void) {
  TIM_Cmd(MOTOR_TIM, DISABLE);
}
