/**
 * Driver for 16x2 Character LCD
 * Model: NHD‐0216BZ‐RN‐YBW
 *
 * @author      Tyler Thompson
 * @date        3/19/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "Timing_Delay.h"

#define __CHARACTER_LCD_H

/* Variables used for the LCD setup */
#define DISP_DATA_PORT          GPIOD
#define DISP_DATA_CLK           RCC_AHB1Periph_GPIOD
#define DISP_CONTROL_PORT       GPIOC
#define DISP_CONTROL_CLK        RCC_AHB1Periph_GPIOC
#define DISP_E_PIN              GPIO_Pin_12
#define DISP_RS_PIN             GPIO_Pin_11
#define DISP_NYB_DELAY          ((uint32_t) 0x5)


/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;

/* Private functions ---------------------------------------------------------*/
static void disp_GPIO_Setup(void);
static void disp_nybble(void);

/* Public functions ---------------------------------------------------------*/
void disp_command(char i);
void disp_init(void);
void disp_write(char i);
void disp_write_string(char *display_string);
void disp_clear(void);
void delay(__IO uint32_t nTime);   
void TimingDelay_Decrement(void);
char int_to_char(uint8_t number);

