/** 
 * Driver for Ultra Sonic Sensor
 * Model: HC-SR4
 *
 * @author      Tyler Thompson
 * @date        3/20/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "UltraSonicSensor.h"

  

/* Private variables ---------------------------------------------------------*/
__IO uint32_t distance1_cm = 0;
__IO uint32_t 1uhIC3ReadValue1 = 0;
__IO uint32_t 1uhIC3ReadValue2 = 0;
__IO uint32_t 1uhCaptureNumber = 0;
__IO uint32_t 1uwCapture1 = 0;
__IO uint32_t 1echo_time = 0;

__IO uint32_t distance2_cm = 0;
__IO uint32_t 2uhIC3ReadValue1 = 0;
__IO uint32_t 2uhIC3ReadValue2 = 0;
__IO uint32_t 2uhCaptureNumber = 0;
__IO uint32_t 2uwCapture1 = 0;
__IO uint32_t 2echo_time = 0;

__IO uint32_t distance3_cm = 0;
__IO uint32_t 3uhIC3ReadValue1 = 0;
__IO uint32_t 3uhIC3ReadValue2 = 0;
__IO uint32_t 3uhCaptureNumber = 0;
__IO uint32_t 3uwCapture1 = 0;
__IO uint32_t 3echo_time = 0;

/**
  * @brief  This function handles Echo TIM global interrupt request.
  * @param  None
  * @retval None
  */
void TIM5_IRQHandler(void)
{ 
  if(TIM_GetITStatus(ECHO_TIM, ECHO1_TIM_IT) == SET) 
  {
    /* Clear TIM Capture compare interrupt pending bit */
    TIM_ClearITPendingBit(ECHO_TIM, ECHO1_TIM_IT);
    if(1uhCaptureNumber == 0){
      /* Get the Input Capture value */
      1uhIC3ReadValue1 = TIM_GetCapture1(ECHO_TIM);
      1uhCaptureNumber = 1;
    }
    else if(1uhCaptureNumber == 1){
      /* Get the Input Capture value */
      1uhIC3ReadValue2 = TIM_GetCapture1(ECHO_TIM); 
      /* Capture computation */
     if (1uhIC3ReadValue2 > 1uhIC3ReadValue1){
        1uwCapture1 = (1uhIC3ReadValue2 - 1uhIC3ReadValue1);
     }
     else if (1uhIC3ReadValue2 < 1uhIC3ReadValue1){
        1uwCapture1 = ((0xFFFFFFFF - 1uhIC3ReadValue1) + 1uhIC3ReadValue2);
     }
     else{
        1uwCapture1 = 0;
     }
     1echo_time = ((1uwCapture1 / (SystemCoreClock / 2000000 )) ); // echo time in micro seconds
     if( 1echo_time >= 36000){ // 35 ms => no object detected
       distance1_cm = 0;
     }
     else {
      distance1_cm = (echo_time / 58);
     }
     1uhCaptureNumber = 0;
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
  GPIO_PinAFConfig(ECHO_GPIO_PORT, ECHO_TIM_PINS, ECHO_TIM_AF);

  /* Enable the TIM global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = ECHO_TIM_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

/* TIM configuration: Input Capture mode ---------------------*/
  TIM_ICInitStructure.TIM_Channel = ECHO_TIM_CHANNELS;
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
