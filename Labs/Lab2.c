/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
 
  /* GPIOD and GPIOA Peripheral clock enable */
  RCC->AHB1ENR = 0x00000009;

  /* Configure PD8 to PD15 as output pushpull mode */
  GPIOD->MODER = 0x55550000;
  GPIOD->OTYPER  = 0x00000000;
  GPIOD->OSPEEDR = 0xFF000000;
  GPIOD->PUPDR = 0x00000000;
  
  //Make sure GPIOA is in input mode
  GPIOA->MODER = 0x00000000;
  
  //When a GPIO pin is high, the LED is off
  //turn off all LEDs
  GPIOD->ODR = 0xFFFF;
  
  while (1)
  {
   
    uint16_t input = GPIOA->IDR;
    
    uint16_t a = input & 0x00F0;
    uint16_t b = input & 0x000F;
  
    a = a >> 4;

    uint16_t output = a + b;
    output = output << 8;
    
    //Invert the output so that it make sense on the LED display
    output = ~output;
    
    GPIOD->ODR = output;
   
  }
}