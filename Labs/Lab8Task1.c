/**
 * ECE4510 Lab8 Task 1
 *
 * @author      Tyler Thompson
 * @date        3/14/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;


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
uint8_t char_array_compare(char *x, char *y, uint8_t size);
void disp_counter(void);

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
  disp_write_string("Hello World\0");
  delay(200);
  disp_clear();
  
      
  while (1)
  {
   disp_counter();
  }
}

void disp_counter(void) {
  char data_array[4] = { 0x30, 0x30, 0x30, '\0' };
  char term_array[4] = { 0x32, 0x31, 0x32, '\0' };
  
  for(uint8_t i = 0; i < 10; i++) {
    if(data_array[0] == 0x3A) {
      data_array[0] = 0x30;
    }
    for(uint8_t j = 0; j < 10; j++) {
      if(data_array[1] == 0x3A) {
      data_array[1] = 0x30;
      }
      for(uint8_t k = 0; k < 10; k++) {
        if(data_array[2] == 0x3A) {
          data_array[2] = 0x30;
        }
        disp_write_string(data_array);
        data_array[2]++;
        delay(250); // 1 second delay
        
        if(char_array_compare(data_array, term_array, 4) == 1){
          return;
        }
        disp_clear();
      }
      data_array[1]++;
    }
    data_array[0]++;
  }
  
}

uint8_t char_array_compare(char *x, char *y, uint8_t size) {
  for(uint8_t i = 0; i < size; i++) {
    if (x[i] != y[i]) {
      return 0;
    }
  }
  return 1;
}

void disp_write_string(char *display_string) {
  disp_clear();
  uint8_t eofs = 0;
  for(uint8_t i = 0; i < 16; i++){
    if(eofs == 0){
      disp_write(display_string[i]);
    }
    else {
      disp_write(0x00);
    }
    
    if(display_string[i] == '\0'){
      eofs = 1;
    }
  }
  //disp_write('\0');
}

void disp_clear(void) {
  GPIOD->ODR = 0x0000;
  GPIO_ResetBits(GPIOA, GPIO_Pin_9); // D/I->HIGH send data
  disp_nybble();
  //disp_write('\0');
  //disp_command(0x06);
  //delay(10);
  //disp_nybble();
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
  //GPIOD->ODR = i << 4;
  //disp_nybble();
  //disp_command(0x10);
}

void disp_command(char i) {
  GPIOD->ODR = i;
  GPIO_ResetBits(GPIOA, GPIO_Pin_9); // D/I->LOW send instruction
  disp_nybble();
  //GPIOD->ODR = i << 4;
  //disp_nybble();
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