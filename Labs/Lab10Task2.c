/**
 * ECE4510 Lab10 Task 2
 *
 * @author      Tyler Thompson
 * @date        3/27/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "Custom_STM32F4_lib\Serial_Com_Link.h"
#include "Custom_STM32F4_lib\DIP_Switch.h"
#include "Custom_STM32F4_lib\Serial_LCD.h"
#include "Custom_STM32F4_lib\Timing_Delay.h"


/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
char send_char = 'T';
char receive_char;
char *send_string = "TEST";
char *receive_string;
uint8_t mode_select = 0;
uint8_t mode_set = 0;


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
 
  /* SPI config */
  //scl_init();
  
  /* DIP switch config, only using switch 1 */
  dip_init();
  
  /* LCD config */
  slcd_init();
  
  slcd_send_string("TEST");
  delay(1000);
  
  /* Infinite Loop */
  while (1)
  { 
    if(dip_get_switch_state(1) != mode_select) {
      mode_set = 0;
    }
    
    mode_select = dip_get_switch_state(1);
    if(mode_select == 1) {
      // Master mode
      if(mode_set == 0) {
        scl_init(0);
        mode_set = 1;
      }
      scl_send_string(send_string);
    }
    else {
      // Slave mode
      if(mode_set == 0) {
        scl_init(1);
        mode_set = 1;
      }
      receive_string = scl_receive_string();
      slcd_send_string(receive_string);
    }
    
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
  {}
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
