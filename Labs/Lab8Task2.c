/**
 * ECE4510 Lab8 Task 2
 *
 * @author      Tyler Thompson
 * @date        3/14/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;
__IO uint32_t raw_temp_value = 0;
__IO uint16_t input_switch = 0;
__IO float temperature = 0;
char temp_array[5] = { 0x30, 0x30, 0xDF, 0x43, '\0' };
uint16_t int_temp;


/* Private functions ---------------------------------------------------------*/
void GPIO_PD0_7_Config(void);
void GPIO_PA8_9_Config(void);
void disp_nybble(void);
void disp_command(char i);
void disp_init(void);
void disp_write(char i);
void delay(uint32_t max_cnt);
void disp_clear(void);
void delay(__IO uint32_t nTime);   
void TimingDelay_Decrement(void);
void disp_write_string(char *display_string);
void ADC_Config(void);
void TIM2_Config(void);
void GPIO_PE8_Config(void);
void convert_temp(void);

/**
  * @brief  Handles the ADC internal interrupt
  * @param  None
  * @retval None
  */
void ADC_IRQHandler(void) {
  raw_temp_value = (ADC_GetConversionValue(ADC2) / 100 ) - 16;
  
}

/**
  * @brief  This function handles TIM2 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{ 
  if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  
    input_switch = ((GPIOE->IDR) & 0x0100) >> 8;
    
    if(input_switch == 1) {
      // temp is fahrenheit
      temperature = (raw_temp_value * 1.8) + 32;
      temp_array[3] = 0x46;
      
    }
    else {
      // temp is celcius
      temperature = raw_temp_value;
      temp_array[3] = 0x43;
      
    }
    
    convert_temp();
  }
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  TimingDelay_Decrement();
}



/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
 
    /* PE8 config for switch */
  GPIO_PE8_Config();
  
  
  /* PD0-7 config for display */
  GPIO_PD0_7_Config();
  
  GPIO_PA8_9_Config();
  
  //enable the systick and load the clock value
  if (SysTick_Config(SystemCoreClock / 200))
  { 
    /* Capture error */ 
  while (1);
 }
  
  /* initialize display */
  disp_init();
  
  // say hi 
  disp_write_string("Hello World");
  delay(200);
  disp_write_string(temp_array);
  
  /* ADC configuration */
  ADC_Config();

  /* Start ADC Software Conversion */ 
  ADC_SoftwareStartConv(ADC2);
    /* Tim2 interrupt config */
  TIM2_Config();
  
 
      
  while (1)
  {
disp_write_string(temp_array);
  delay(250);
  
  }
}

// convert the temperature into a char array
void convert_temp(void) {
  int_temp = (uint16_t)temperature;
  temp_array[0] = ((int_temp /10) % 10) | 0x30;
  temp_array[1] = (int_temp % 10) | 0x30;
  
}


void disp_write_string(char *display_string) {
  disp_clear();
  
  for(uint8_t i = 0; i < 16; i++){
    if(display_string[i] == '\0'){
      return;
    //  disp_write(0x00);
    }
      disp_write(display_string[i]);
  }

}

void disp_clear(void) {
  disp_command(0x01);
  disp_command(0x0C);
}

void disp_init(void){
  GPIO_ResetBits(GPIOA, GPIO_Pin_8); // E-> LOW
  delay(100);
  disp_command(0x30);
  delay(30);
  disp_command(0x30);
  delay(10);
  disp_command(0x30);;
  delay(10);
  disp_command(0x38);
  disp_command(0x10);
  disp_command(0x0F);
  disp_command(0x06);
  disp_clear();
}

void disp_write(char i) {
  GPIOD->ODR = i;
  GPIO_SetBits(GPIOA, GPIO_Pin_9); // D/I->HIGH send data
  disp_nybble();
}

void disp_command(char i) {
  GPIOD->ODR = i;
  GPIO_ResetBits(GPIOA, GPIO_Pin_9); // D/I->LOW send instruction
  disp_nybble();
}

void disp_nybble(void){
  GPIO_SetBits(GPIOA, GPIO_Pin_8); // E-> HIGH
  delay(1);
  GPIO_ResetBits(GPIOA, GPIO_Pin_8); // E-> LOW
}


/**
  * @brief  Sets up PD0-7 for display
  * @param  None
  * @retval None
  */
void GPIO_PD0_7_Config(void) {
 GPIO_InitTypeDef GPIO_InitStructure;

  /* GPIOD clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  
  /* GPIOD Configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
}

/**
  * @brief  Sets up PA8-9 for display
  * @param  None
  * @retval None
  */
void GPIO_PA8_9_Config(void) {
 GPIO_InitTypeDef GPIO_InitStructure;

  /* GPIOD clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
  /* GPIOD Configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
}

/**
  * @brief  ADC2 channel10
  * @note   This function Configure the ADC peripheral  
            1) Enable peripheral clocks
            3) Configure ADC Channel10 pin as analog input
            4) Configure ADC1 Channel10 
  * @param  None
  * @retval None
  */
void ADC_Config(void)
{
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  GPIO_InitTypeDef      GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable ADC2 and GPIO clocks ****************************************/ 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
  

  /* Configure ADC2 Channel10 pin as analog input on PC0 ******************************/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
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

  /* ADC2 Init ****************************************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC2, &ADC_InitStructure);

  /* ADC2 regular channel10 configuration *************************************/
  ADC_RegularChannelConfig(ADC2, ADC_Channel_10, 1, ADC_SampleTime_3Cycles);

  ADC_ITConfig(ADC2,ADC_IT_EOC,ENABLE);
  /* Enable ADC2 */
  ADC_Cmd(ADC2, ENABLE);
  
 
}

/**
  * @brief  Configure the TIM2 interrupt.
  * @param  None
  * @retval None
  */
void TIM2_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
  
  /* TIM2 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  
  /* Time base configuration fo 40Hz or 25ms intervals */
  TIM_TimeBaseStructure.TIM_Period = 210;
  TIM_TimeBaseStructure.TIM_Prescaler = 10000;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* TIM2 enable counter */
  TIM_Cmd(TIM2, ENABLE);
  
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  TIM_SetCounter(TIM2, 0);
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  
  /* Enable the TIM2 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


/**
  * @brief  Sets up PE8 to read the switch
  * @param  None
  * @retval None
  */
void GPIO_PE8_Config(void) {
 GPIO_InitTypeDef GPIO_InitStructure;

  /* GPIOE clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  
  /* GPIOE Configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0)
  { 
    TimingDelay--;
  }
}