/**
 * ECE4510 PreLab 3 Task Two
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
    EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    
    /* Enable clock for GPIOD */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    /* Enable clock for SYSCFG */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
    /* Set pin as input */
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    /* use PA3 for EXTI_Line3 */
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource3);
    
    /* PA3 is connected to EXTI_Line3 */
    EXTI_InitStruct.EXTI_Line = EXTI_Line3;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStruct);
 
 
    /* PA3 is connected to EXTI_Line3, which has EXTI3_IRQn vector */
    NVIC_InitStruct.NVIC_IRQChannel = EXTI3_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}
 
 
/**
  * @brief  Configure PD8 to PD15 as outputs
  * @param  None
  * @retval None
  */
void Configure_PD8_15_Output(void) {
  GPIO_InitTypeDef GPIO_InitStructure;

	
  /* Enable clock for GPIOD */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    
  /* Configure PD8 to PD15 as output pushpull mode */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
}
 

/**
  * @brief  Handle PA3 interrupt
  * @param  None
  * @retval None
  */
void EXTI3_IRQHandler(void) {

    if (EXTI_GetITStatus(EXTI_Line3) != RESET) {
        
        uint16_t count = GPIOD->ODR;
        
        //negatine count accounts for LEDs be reverse polarity
        count = ~count;
        count = count >> 8;
        count++;
        if ( count == 0x00FF ) {
          count = 0x0000;
        }
        count = count << 8;
        count = ~count;
        GPIOD->ODR = count;
     
        
        /* Clear interrupt flag */
        EXTI_ClearITPendingBit(EXTI_Line3);
    }
   
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
    
    /* Configure PD8 to PD15 as outputs */
    Configure_PD8_15_Output();
	
    GPIOD->ODR = 0x0000;
    while (1) {
    }
}
