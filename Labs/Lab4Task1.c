/**
 * ECE4510 Lab4 Task 1
 *
 * @author      Tyler Thompson
 * @date        2/1/2018
 */


#include "stm32f4xx_it.h"
#include "main.h"

/* private variables */
GPIO_InitTypeDef GPIO_InitStructure;
static __IO uint32_t TimingDelay;

/* private functions */
static void Delay(__IO uint32_t nTime);   
void TimingDelay_Decrement(void);
static void configure_PE0();

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
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
 
  //setup PE0 as an output
  configure_PE0();
 
  //enable the systick and load the clock value
  if (SysTick_Config(SystemCoreClock / 200))
  { 
    /* Capture error */ 
  while (1);
 }

  while (1)
  {
    

    /* PE0=1 */
    GPIOE->ODR=0xFFFF;

    /* Insert 5 ms delay */
    Delay(1);
    
    /* PE0=0*/
    GPIOE->ODR=0x0000;
    /* Insert 5 ms delay */
    Delay(1);
  }
}

/**
  * @brief  Configure PE0 as an output
  * @param  None
  * @retval None
  */
void configure_PE0(void) {
  /* enable GPIOE */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

      /* Configure PE0 pin as ouput */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
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
