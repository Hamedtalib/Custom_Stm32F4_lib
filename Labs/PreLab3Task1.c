/**
 * ECE4510 PreLab 3 Task One
 *
 * @author      Tyler Thompson
 * @date        1/30/2018
 */


#include "main.h"
 

/**
  * @brief  Configure PA3 as an interrupt pin
  * @param  None
  * @retval None
  */
void Configure_PA3(void) {

    GPIO_InitTypeDef GPIO_InitStruct;
    
    /* Enable clock for GPIOD */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    
    /* Set pin as input */
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    
  
}
 
 
 
 /**
  * @brief  Configure PD12 as output
  * @param  None
  * @retval None
  */
void Configure_PD12(void) {
  GPIO_InitTypeDef GPIO_InitStructure;

	
  /* Enable clock for GPIOD */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    
  /* Configure PD12 as output pushpull mode */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
}
 


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void) {
    
    /* System init */
    SystemInit();
	
    /* Configure PA3 as interrupt */
    Configure_PA3();
    
    /* Configure PD12 as outputs */
    Configure_PD12();
	
    
    while (1) {
     uint16_t x = GPIOA->IDR;
     x = (x << 9);
     GPIOD->ODR = x; 
    }
}
