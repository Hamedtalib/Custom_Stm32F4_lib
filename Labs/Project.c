/**
 * ECE4510 Final Project
 *
 * @author      Tyler Thompson
 * @date        4/5/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "Custom_STM32F4_lib\Timing_Delay.h"
#include "Custom_STM32F4_lib\DIP_Switch.h"
#include "Custom_STM32F4_lib\Serial_LCD.h"
#include "Custom_STM32F4_lib\Motor_Encoder.h"
#include "Custom_STM32F4_lib\PWM_Motor_Control.h"
#include "Custom_STM32F4_lib\UltraSonicSensor.h"
#include "Custom_STM32F4_lib\LED_Panel_X8.h"
#include "Custom_STM32F4_lib\Buzzer.h"


/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void) {
 
  while (1) { 

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

  /* Infinite loop */
  while (1)
  {}
}
#endif

