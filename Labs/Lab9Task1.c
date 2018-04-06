/**
 * ECE4510 Lab9 Task 1
 *
 * @author      Tyler Thompson
 * @date        3/20/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "Custom_STM32F4_lib\Character_LCD.h"

uint8_t char_array_compare(char *x, char *y, uint8_t size);
void disp_counter(void);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  
 disp_init();
  
  while (1)
  {
   disp_counter();
  }
}

void disp_counter(void) {
  char term_array[4] = { 0x30, 0x30, 0x30, '\0' };
  char data_array[4] = { 0x33, 0x30, 0x30, '\0' };
   
  for(uint8_t i = 0; i < 10; i++) {
    if(data_array[0] == 0x3A) {
      data_array[0] = 0x30;
    }
    for(uint8_t j = 0; j < 10; j++) {
      if(data_array[1] == 0x3A) {
      data_array[1] = 0x30;
      }
      for(uint8_t k = 0; k < 10; k++) {
        disp_write_string(data_array);
        data_array[2]--;
        if(data_array[2] == 0x3A) {
          data_array[2] = 0x30;
        }
        delay(250); // 1 second delay
        if(char_array_compare(data_array, term_array, 4) == 1){
          return;
        }       
      }
      data_array[1]--;
    }
    data_array[0]--;
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