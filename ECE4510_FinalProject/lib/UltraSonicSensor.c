/** 
 * Driver for Ultra Sonic Sensor
 * Model: HC-SR4
 *
 * @author      Tyler Thompson
 * @date        3/20/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "UltraSonicSensor.h"

/* Private variables ---------------------------------------------------------*/
static __IO uint64_t distance_cm = 0;
static __IO uint64_t distance_in = 0;
static __IO uint64_t uhIC3ReadValue1 = 0;
static __IO uint64_t uhIC3ReadValue2 = 0;
static __IO uint64_t uhCaptureNumber = 0;
static __IO uint64_t uwCapture1 = 0;
static __IO uint64_t echo_time = 0;

/**
  * @brief  This function handles Echo TIM global interrupt request.
  * @param  None
  * @retval None
  */
void ECHO_TIM_IRQ_HANDLE(void)
{ 
  if(TIM_GetITStatus(ECHO_TIM, ECHO_TIM_IT) == SET) 
  {
    /* Clear TIM Capture compare interrupt pending bit */
    TIM_ClearITPendingBit(ECHO_TIM, ECHO_TIM_IT);
    if(uhCaptureNumber == 0)
    {
      /* Get the Input Capture value */
      uhIC3ReadValue1 = TIM_GetCapture1(ECHO_TIM);
      uhCaptureNumber = 1;
    }
    else if(uhCaptureNumber == 1)
    {
      /* Get the Input Capture value */
      uhIC3ReadValue2 = TIM_GetCapture1(ECHO_TIM); 
      
      /* Capture computation */
     if (uhIC3ReadValue2 > uhIC3ReadValue1)
     {
        uwCapture1 = (uhIC3ReadValue2 - uhIC3ReadValue1);
     }
     else if (uhIC3ReadValue2 < uhIC3ReadValue1)
     {
        uwCapture1 = ((0xFFFFFFFF - uhIC3ReadValue1) + uhIC3ReadValue2);
     }
     else
     {
        uwCapture1 = 0;
     }
     
     echo_time = ((uwCapture1 / (SystemCoreClock / 2000000 )) ); // echo time in micro seconds
     if( echo_time >= 36000){ // 35 ms => no object detected
       distance_cm = 0xffffffff;
      distance_in = 0xffffffff;
     }
     else {
      distance_cm = (echo_time / 58);
      distance_in = (echo_time / 148);
     }
     uhCaptureNumber = 0;
      
    }  
    
  }
}

/**
  * @brief  Get the distance in centimeters
  * @param  None
  * @retval None
  */
uint32_t ultrasonic_get_distance_cm(void) {
  return distance_cm;
}

/**
  * @brief  Get the distance in inches
  * @param  None
  * @retval None
  */
uint32_t ultrasonic_get_distance_in(void) {
  return distance_in;
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
  GPIO_InitStructure.GPIO_Pin =  ECHO_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(ECHO_GPIO_PORT, &GPIO_InitStructure);

  /* Connect TIM pins to AF1 */
  GPIO_PinAFConfig(ECHO_GPIO_PORT, ECHO_TIM_PIN, ECHO_TIM_AF);

  /* Enable the TIM global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = ECHO_TIM_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

/* TIM configuration: Input Capture mode ---------------------*/
  TIM_ICInitStructure.TIM_Channel = ECHO_TIM_CHANNEL;
  TIM_ICInitStructure.TIM_ICPolarity =  TIM_ICPolarity_BothEdge;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;

  TIM_ICInit(ECHO_TIM, &TIM_ICInitStructure);

  /* TIM enable counter */
  TIM_Cmd(ECHO_TIM, ENABLE);

  /* Enable the CC Interrupt Request */
  TIM_ITConfig(ECHO_TIM, ECHO_TIM_IT, ENABLE);

}


/**
  * @brief  Configure the TIM and GPIO port for trig pin
  * @note   Output signal will initialize to be disabled. Use utrasonic_trig() to create a trigger signal.
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

  TRIG_OC_INIT(TRIG_TIM, &TIM_OCInitStructure);
  
   /* TIM enable counter */
  TIM_Cmd(TRIG_TIM, ENABLE);
    

}
