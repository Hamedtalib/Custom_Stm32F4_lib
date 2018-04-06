/**
 * ECE4510 Lab10 Task 1
 *
 * @author      Tyler Thompson
 * @date        3/27/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "Custom_STM32F4_lib\Serial_Com_Link.h"
#include "Custom_STM32F4_lib\DIP_Switch.h"


/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
char send_char = 'T';
char receive_char;
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
      scl_send_char(send_char);
    }
    else {
      // Slave mode
      if(mode_set == 0) {
        scl_init(1);
        mode_set = 1;
      }
      receive_char = scl_receive_char();
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
