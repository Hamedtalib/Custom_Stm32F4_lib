/** 
 * Driver for Ultra Sonic Sensor
 * Model: HC-SR4
 *
 * @author      Tyler Thompson
 * @date        3/20/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "UltraSonicSensorX3.h"

  

/* Private variables ---------------------------------------------------------*/
__IO uint32_t distance1_cm = 0;
__IO uint32_t ic_read_value_1_1 = 0;
__IO uint32_t ic_read_value_2_1 = 0;
__IO uint32_t capture_number_1 = 0;
__IO uint32_t capture_1 = 0;
__IO uint32_t echo_time_1 = 0;

__IO uint32_t distance2_cm = 0;
__IO uint32_t ic_read_value_1_2 = 0;
__IO uint32_t ic_read_value_2_2 = 0;
__IO uint32_t capture_number_2 = 0;
__IO uint32_t capture_2 = 0;
__IO uint32_t echo_time_2 = 0;

__IO uint32_t distance3_cm = 0;
__IO uint32_t ic_read_value_1_3 = 0;
__IO uint32_t ic_read_value_2_3 = 0;
__IO uint32_t capture_number_3 = 0;
__IO uint32_t capture_3 = 0;
__IO uint32_t echo_time_3 = 0;

/**
  * @brief  This function handles Echo TIM global interrupt request.
  * @param  None
  * @retval None
  */
void TIM5_IRQHandler(void)
{ 
  // sensor 1
  if(TIM_GetITStatus(ECHO_TIM, ECHO1_TIM_IT) == SET) {
    /* Clear TIM Capture compare interrupt pending bit */
    TIM_ClearITPendingBit(ECHO_TIM, ECHO1_TIM_IT);
    if(capture_number_1 == 0){
      /* Get the Input Capture value */
      ic_read_value_1_1 = TIM_GetCapture1(ECHO_TIM);
      capture_number_1 = 1;
    }
    else if(capture_number_1 == 1){
      /* Get the Input Capture value */
      ic_read_value_2_1 = TIM_GetCapture1(ECHO_TIM); 
      /* Capture computation */
     if (ic_read_value_2_1 > ic_read_value_1_1){
        capture_1 = (ic_read_value_2_1 - ic_read_value_1_1);
     }
     else if (ic_read_value_2_1 < ic_read_value_1_1){
        capture_1 = ((0xFFFFFFFF - ic_read_value_1_1) + ic_read_value_2_1);
     }
     else{
        capture_1 = 0;
     }
     echo_time_1 = ((capture_1 / (SystemCoreClock / 2000000 )) ); // echo time in micro seconds
  //   if( echo_time_1 >= 36000){ // 35 ms => no object detected
   //    distance1_cm = 0;
   //  }
   //  else {
      distance1_cm = (echo_time_1 / 58);
   //  }
     capture_number_1 = 0;
    }  
  }
  
  // sensor 2
  if(TIM_GetITStatus(ECHO_TIM, ECHO2_TIM_IT) == SET) {
    /* Clear TIM Capture compare interrupt pending bit */
    TIM_ClearITPendingBit(ECHO_TIM, ECHO2_TIM_IT);
    if(capture_number_2 == 0){
      /* Get the Input Capture value */
      ic_read_value_1_2 = TIM_GetCapture2(ECHO_TIM);
      capture_number_2 = 1;
    }
    else if(capture_number_2 == 1){
      /* Get the Input Capture value */
      ic_read_value_2_2 = TIM_GetCapture2(ECHO_TIM); 
      /* Capture computation */
     if (ic_read_value_2_2 > ic_read_value_1_2){
        capture_2 = (ic_read_value_2_2 - ic_read_value_1_2);
     }
     else if (ic_read_value_2_2 < ic_read_value_1_2){
        capture_2 = ((0xFFFFFFFF - ic_read_value_1_2) + ic_read_value_2_2);
     }
     else{
        capture_2 = 0;
     }
     echo_time_2 = ((capture_2 / (SystemCoreClock / 2000000 )) ); // echo time in micro seconds
   //  if( echo_time_2 >= 36000){ // 35 ms => no object detected
   //    distance2_cm = 0;
   //  }
   //  else {
      distance2_cm = (echo_time_2 / 58);
  //   }
     capture_number_2 = 0;
    }  
  }
  
  // sensor 3
  if(TIM_GetITStatus(ECHO_TIM, ECHO3_TIM_IT) == SET) {
    /* Clear TIM Capture compare interrupt pending bit */
    TIM_ClearITPendingBit(ECHO_TIM, ECHO3_TIM_IT);
    if(capture_number_3 == 0){
      /* Get the Input Capture value */
      ic_read_value_1_3 = TIM_GetCapture3(ECHO_TIM);
      capture_number_3 = 1;
    }
    else if(capture_number_3 == 1){
      /* Get the Input Capture value */
      ic_read_value_2_3 = TIM_GetCapture3(ECHO_TIM); 
      /* Capture computation */
     if (ic_read_value_2_3 > ic_read_value_1_3){
        capture_3 = (ic_read_value_2_3 - ic_read_value_1_3);
     }
     else if (ic_read_value_2_3 < ic_read_value_1_3){
        capture_3 = ((0xFFFFFFFF - ic_read_value_1_3) + ic_read_value_2_3);
     }
     else{
        capture_3 = 0;
     }
     echo_time_3 = ((capture_3 / (SystemCoreClock / 2000000 )) ); // echo time in micro seconds
  //   if( echo_time_3 >= 36000){ // 35 ms => no object detected
  //     distance3_cm = 0;
  //   }
  //   else {
      distance3_cm = (echo_time_3 / 58);
  //   }
     capture_number_3 = 0;
    }  
  }
}

/**
  * @brief  Get the distance in centimeters from sensor 1
  * @param  None
  * @retval None
  */
uint32_t ultrasonic1_get_distance_cm(void) {
  return distance1_cm;
}

/**
  * @brief  Get the distance in centimeters from sensor 2
  * @param  None
  * @retval None
  */
uint32_t ultrasonic2_get_distance_cm(void) {
  return distance2_cm;
}

/**
  * @brief  Get the distance in centimeters from sensor 3
  * @param  None
  * @retval None
  */
uint32_t ultrasonic3_get_distance_cm(void) {
  return distance3_cm;
}

/**
  * @brief  Initialize the Ultra Sonic sensor
  * @param  None
  * @retval None
  */
void ultrasonic_init(void) {
  _output_compare_config();
  _input_capture_config();
}

/**
  * @brief  Configure the TIM and GPIO port for the echo pin
  * @param  None
  * @retval None
  */
static void _input_capture_config(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_ICInitTypeDef  TIM_ICInitStructure;

  /* TIM clock enable */
  RCC_APB1PeriphClockCmd(ECHO_TIM_CLK, ENABLE);

  /* GPIO clock enable */
  RCC_AHB1PeriphClockCmd(ECHO_GPIO_CLK, ENABLE);

  /* TIM channel configuration */
  GPIO_InitStructure.GPIO_Pin =  ECHO_GPIO_PINS;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(ECHO_GPIO_PORT, &GPIO_InitStructure);

  /* Connect TIM pins to AF1 */
  GPIO_PinAFConfig(ECHO_GPIO_PORT, ECHO1_TIM_PIN, ECHO_TIM_AF);
  GPIO_PinAFConfig(ECHO_GPIO_PORT, ECHO2_TIM_PIN, ECHO_TIM_AF);
  GPIO_PinAFConfig(ECHO_GPIO_PORT, ECHO3_TIM_PIN, ECHO_TIM_AF);

  
  /* Enable the TIM global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = ECHO_TIM_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

/* TIM configuration: Input Capture mode ---------------------*/
  TIM_ICInitStructure.TIM_Channel = ECHO1_TIM_CHANNEL;
  TIM_ICInitStructure.TIM_ICPolarity =  TIM_ICPolarity_BothEdge;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;

  TIM_ICInit(ECHO_TIM, &TIM_ICInitStructure);
  
  TIM_ICInitStructure.TIM_Channel = ECHO2_TIM_CHANNEL;
  TIM_ICInitStructure.TIM_ICPolarity =  TIM_ICPolarity_BothEdge;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;

  TIM_ICInit(ECHO_TIM, &TIM_ICInitStructure);
  
  TIM_ICInitStructure.TIM_Channel = ECHO3_TIM_CHANNEL;
  TIM_ICInitStructure.TIM_ICPolarity =  TIM_ICPolarity_BothEdge;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;

  TIM_ICInit(ECHO_TIM, &TIM_ICInitStructure);

  /* TIM enable counter */
  TIM_Cmd(ECHO_TIM, ENABLE);

  /* Enable the CC Interrupt Request */
  TIM_ITConfig(ECHO_TIM, ECHO1_TIM_IT, ENABLE);
  TIM_ITConfig(ECHO_TIM, ECHO2_TIM_IT, ENABLE);
  TIM_ITConfig(ECHO_TIM, ECHO3_TIM_IT, ENABLE);

}


/**
  * @brief  Configure the TIM and GPIO port for trig pin
  * @param  None
  * @retval None
  */
static void _output_compare_config(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;

  /* GPIO clock enable */
  RCC_AHB1PeriphClockCmd(TRIG_GPIO_CLK, ENABLE);

  /* GPIO Configuration */
  GPIO_InitStructure.GPIO_Pin = TRIG_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;

  GPIO_Init(TRIG_GPIO_PORT, &GPIO_InitStructure); 

  /* Connect TIM pins to AF2 */  
  GPIO_PinAFConfig(TRIG_GPIO_PORT, TRIG_TIM_PIN, TRIG_TIM_AF);
/* TIM clock enable */
  RCC_APB1PeriphClockCmd(TRIG_TIM_CLK, ENABLE);

  /* Compute the value to be set in ARR regiter to generate signal frequency */
  uint32_t TimerPeriod = 4200000;
  
  //duty cycle
  uint16_t CCR1_Val = (uint32_t) (((float) 0.0016 * (TimerPeriod - 1)) / 10);

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TRIG_TIM, &TIM_TimeBaseStructure); 

  /* PWM1 Mode configuration */
  TIM_OCInitStructure.TIM_OCMode = TRIG_PWM_CHANNEL;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC3Init(TRIG_TIM, &TIM_OCInitStructure);
  
   /* TIM enable counter */
  TIM_Cmd(TRIG_TIM, ENABLE);
    

}
