/**
 * ECE4510 Lab8 Task 3
 *
 * @author      Tyler Thompson
 * @date        3/14/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;
I2C_InitTypeDef  I2C_InitStructure;
RCC_ClocksTypeDef RCC_Clocks;
uint8_t temp_data1 = 0;
uint8_t temp_data2 = 0;
char temp_array[5] = { 0x30, 0x30, 0xDF, 0x43, '\0' };
uint16_t int_temp;
__IO float temperature = 0;
__IO uint16_t input_switch = 0;



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
static void I2C_Config(void);
static void GPIO_PE8_Config(void);
static void TIM2_Config(void);
void I2C_setup_temp(void);
void I2C_read_temp(void);
void convert_temp(void);

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
    I2C_read_temp();
    if(input_switch == 1) {
      // temp is fahrenheit
      temperature = (temp_data1 * 1.8) + 32;
      temp_array[3] = 0x46;
      
    }
    else {
      // temp is celcius
      temperature = temp_data1;
      temp_array[3] = 0x43;
      
    }
    
    convert_temp();
    
    
    
    
  }
}


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
 
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
  //disp_write_string("Hello World");
 // delay(200);
  //disp_clear();
  
  /* I2C configuration */
  I2C_Config(); 

  I2C_setup_temp();  
  
  /* PE8 config for switch */
  GPIO_PE8_Config();
  
  /* Tim2 Config for interrupts */
  TIM2_Config();
      
  
  
  
  
  while (1)
  {
   disp_write_string(temp_array);
   delay(100);
  }
}

// convert the temperature into a char array
void convert_temp(void) {
  int_temp = (uint16_t)temperature;
  temp_array[0] = ((int_temp /10) % 10) | 0x30;
  temp_array[1] = (int_temp % 10) | 0x30;
  
}

void I2C_setup_temp(void) {
    //setup
  while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
  I2C_GenerateSTART(I2C1, ENABLE);
  
  while ((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)));
    
  I2C_Send7bitAddress(I2C1, 0x90, I2C_Direction_Transmitter);
  
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) );
  
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING) );
  
  I2C_SendData(I2C1, 0x01);
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING) );
  I2C_SendData(I2C1, 0x60);
  
  I2C_GenerateSTOP(I2C1, ENABLE);
}

void I2C_read_temp(void) {
    //reading
  I2C_GenerateSTART(I2C1, ENABLE);
  
  while ((!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)));
    
  I2C_Send7bitAddress(I2C1, 0x90, I2C_Direction_Transmitter);
  
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) );
  
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING) );
  
  I2C_SendData(I2C1, 0x00);
  
  I2C_GenerateSTART(I2C1, ENABLE);
  
  while ((!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)));
    
  I2C_Send7bitAddress(I2C1, 0x90, I2C_Direction_Receiver);
  
  while ((!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)));
          
  I2C_AcknowledgeConfig(I2C1, ENABLE);
  
  while ((!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)));
          
  temp_data1 = I2C_ReceiveData(I2C1);
  
  I2C_AcknowledgeConfig(I2C1, DISABLE);
  
  while ((!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)));
          
  temp_data2 = I2C_ReceiveData(I2C1);
  
  I2C_GenerateSTOP(I2C1, ENABLE);
}

/**
  * @brief  Enables the I2C Clock and configures the different GPIO ports.
  * @param  None
  * @retval None
  */
static void I2C_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
    
  /* RCC Configuration */
  /*I2C Peripheral clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
  
  /*SDA/SCL GPIO clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  
  /* Reset I2C1 IP */
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
  
  /* Release reset signal of I2C1 IP */
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);
  
  /* GPIO Configuration */
  /*Configure I2C SCL pin PB6*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /*Configure I2C SDA pin PB9*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
    
  /* Connect PXx to I2C_SCL */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);
  
  /* Connect PXx to I2C_SDA */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);
   
   /* Configure I2C Filters */
   I2C_AnalogFilterCmd(I2C1,ENABLE);
   I2C_DigitalFilterConfig(I2C1,0x0F);
   
  /* I2C ENABLE */
  I2C_Cmd(I2C1, ENABLE);
   
   
   /* I2C Struct Initialize */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2; //50%
  I2C_InitStructure.I2C_OwnAddress1 = 0xA0;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_ClockSpeed = 100000;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  
  /* I2C Initialize */
  I2C_Init(I2C1, &I2C_InitStructure);
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

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif