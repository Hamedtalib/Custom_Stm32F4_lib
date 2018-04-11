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
 __forward_init();
 __reverse_init();
}

/**
  * @brief  Initialize the forward PWM signal
  * @param  None
  * @retval None
  */
static void __forward_init(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;

  /* GPIO clock enable */
  RCC_AHB1PeriphClockCmd(MOTORF_GPIO_CLK, ENABLE);

  /* GPIO Configuration */
  GPIO_InitStructure.GPIO_Pin = MOTORF_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

  GPIO_Init(MOTORF_GPIO_PORT, &GPIO_InitStructure); 

  /* Connect TIM pins to AF2 */  
  GPIO_PinAFConfig(MOTORF_GPIO_PORT, MOTORF_TIM_PIN, MOTORF_TIM_AF);
  
/* TIM clock enable */
  RCC_APB1PeriphClockCmd(MOTORF_TIM_CLK, ENABLE);

  /* Compute the value to be set in ARR regiter to generate signal frequency */
  uint32_t TimerPeriod = (SystemCoreClock / (MOTORF_INIT_FREQ * 2)) - 1;
  
  //duty cycle
  uint16_t CCR1_Val = (uint16_t) (((uint32_t) MOTORF_INIT_DUTY * (TimerPeriod + 1)) / 100);

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(MOTORF_TIM, &TIM_TimeBaseStructure); 

  /* PWM1 Mode configuration */
  TIM_OCInitStructure.TIM_OCMode = MOTORF_PWM_CHANNEL;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  MOTORF_OC_INIT(MOTORF_TIM, &TIM_OCInitStructure);
}

/**
  * @brief  Initialize the reverse PWM signal
  * @param  None
  * @retval None
  */
static void __reverse_init(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;

  /* GPIO clock enable */
  RCC_AHB1PeriphClockCmd(MOTORR_GPIO_CLK, ENABLE);

  /* GPIO Configuration */
  GPIO_InitStructure.GPIO_Pin = MOTORR_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;

  GPIO_Init(MOTORR_GPIO_PORT, &GPIO_InitStructure); 

  /* Connect TIM pins to AF2 */  
  GPIO_PinAFConfig(MOTORR_GPIO_PORT, MOTORR_TIM_PIN, MOTORR_TIM_AF);
  
/* TIM clock enable */
  RCC_APB2PeriphClockCmd(MOTORR_TIM_CLK, ENABLE);

  /* Compute the value to be set in ARR regiter to generate signal frequency */
  uint32_t TimerPeriod = (SystemCoreClock / (MOTORR_INIT_FREQ )) - 1;
  
  //duty cycle
  uint16_t CCR1_Val = (uint16_t) (((uint32_t) MOTORR_INIT_DUTY * (TimerPeriod + 1)) / 100);

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(MOTORR_TIM, &TIM_TimeBaseStructure); 

  /* PWM1 Mode configuration */
  TIM_OCInitStructure.TIM_OCMode = MOTORR_PWM_CHANNEL;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  MOTORR_OC_INIT(MOTORR_TIM, &TIM_OCInitStructure);
}

/**
  * @brief  Set the duty cycle for the motor
  * @param  duty - The duty cycle to set
  * @param  direction - 0 => forward, 1 => reverse
  * @retval None
  */
void pwmmotor_set_duty(uint32_t duty, uint8_t direction) {
  TIM_OCInitTypeDef  TIM_OCInitStructure;

  uint32_t TimerPeriod = 0;
   if (direction == 1){
    TimerPeriod = (SystemCoreClock / (MOTORR_INIT_FREQ)) - 1;
  }
  else {
    TimerPeriod = (SystemCoreClock / (MOTORF_INIT_FREQ * 2)) - 1;
  } 
  
  //duty cycle
  uint16_t CCR1_Val = (uint16_t) (((uint32_t) duty * (TimerPeriod + 1)) / 100);

  /* PWM1 Mode configuration */
  if (direction == 1){
    TIM_OCInitStructure.TIM_OCMode = MOTORR_PWM_CHANNEL;
  }
  else {
    TIM_OCInitStructure.TIM_OCMode = MOTORF_PWM_CHANNEL;
  }
  
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  if (direction == 1){
    MOTORR_OC_INIT(MOTORR_TIM, &TIM_OCInitStructure);
  }
  else {
    MOTORF_OC_INIT(MOTORF_TIM, &TIM_OCInitStructure);
  }
  
}

/**
  * @brief  Enable the motor
  * @param  direction - 0 => forward, 1 => reverse
  * @retval None
  */
void pwmmotor_enable(uint8_t direction) {
  if (direction == 1){
    __reverse_init();
    TIM_Cmd(MOTORR_TIM, ENABLE);
  }
  else {
    __forward_init();
    TIM_Cmd(MOTORF_TIM, ENABLE);
  }
}

/**
  * @brief  Disable to motor
  * @param  direction - 0 => forward, 1 => reverse
  * @retval None
  */
void pwmmotor_disable(uint8_t direction) {
  GPIO_InitTypeDef GPIO_InitStructure;
  if (direction == 1){
    TIM_Cmd(MOTORR_TIM, DISABLE);
    /* GPIO Configuration */
  GPIO_InitStructure.GPIO_Pin = MOTORR_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

  GPIO_Init(MOTORR_GPIO_PORT, &GPIO_InitStructure); 
  GPIO_ResetBits(MOTORR_GPIO_PORT, MOTORR_GPIO_PIN);
  }
  else {
    TIM_Cmd(MOTORF_TIM, DISABLE);
      /* GPIO Configuration */
  GPIO_InitStructure.GPIO_Pin = MOTORF_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

  GPIO_Init(MOTORF_GPIO_PORT, &GPIO_InitStructure); 
  GPIO_ResetBits(MOTORF_GPIO_PORT, MOTORF_GPIO_PIN);
  }
}
