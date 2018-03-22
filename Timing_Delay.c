/**
 * Various timeing delay functions
 *
 * @author      Tyler Thompson
 * @date        3/21/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/**
  * @brief  
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  TimingDelay_Decrement();
}

void delay_init(void) {
//enable the systick and load the clock value
  if (SysTick_Config(SystemCoreClock / 100))
  {
    /* Capture error */
  while (1);
 }

}

/**
  * @brief  Inserts a delay time.
  * @param  micro_time: specifies the delay time length, in microseconfd
  * @retval None
  */
void delay_micro(__IO uint32_t micro_time) { 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}
/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void delay(uint32_t nTime) { 
  __IO uint32_t micro_time = nTime * 10;
  delay_micro(micro_time);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
static void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0)
  { 
    TimingDelay--;
  }
