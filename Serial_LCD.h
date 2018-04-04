/**
 * Driver for serial LCD
 * Model: ST7920
 *
 * @author      Tyler Thompson
 * @date        3/27/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define __SERIAL_LCD_H

#define SLCD_GPIO_PORT	        GPIOB
#define SLCD_GPIO_CLK	        RCC_AHB1Periph_GPIOB

#define SLCD_SCK_PIN            GPIO_Pin_13 // SPI SCK -> CLK 10
#define SLCD_SCK_SOURCE         GPIO_PinSource13

#define SLCD_MOSI_PIN           GPIO_Pin_15 // SPI MOSI -> SID 15
#define SLCD_MOSI_SOURCE        GPIO_PinSource15

#define SLCD_SPI                SPI2
#define SLCD_SPI_CLK            RCC_APB1Periph_SPI2
#define SLCD_SPI_CLK_INIT       RCC_APB1PeriphClockCmd
#define SLCD_SPI_IRQ            SPI2_IRQn
#define SLCD_SPI_IRQHANDEL      SPI2_IRQHandler
#define SLCD_SPI_AF             GPIO_AF_SPI2


/* Private variables ---------------------------------------------------------*/

/* Public functions ---------------------------------------------------------*/
void SLCD_SPI_IRQHANDLER(void);
void slcd_init(void);
void slcd_clear(void);
void slcd_send_char(char data);
void slcd_send_string(char *data);


/* Private functions ---------------------------------------------------------*/
static void slcd_send_data(char cmd_bits, char higher_bits, char lower_bits);
