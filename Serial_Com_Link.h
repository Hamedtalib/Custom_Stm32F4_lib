/**
 * Driver for serial commuication between two devices
 *
 * @author      Tyler Thompson
 * @date        3/27/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define __SERIAL_LCD_H

#define SCL_GPIO_PORT	        GPIOA
#define SCL_GPIO_CLK	        RCC_AHB1Periph_GPIOA

#define SCL_SCK_PIN            GPIO_Pin_5 // SPI SCK -> CLK
#define SCL_SCK_SOURCE         GPIO_PinSource5

#define SCL_MOSI_PIN           GPIO_Pin_7 // SPI MOSI -> TX
#define SCL_MOSI_SOURCE        GPIO_PinSource7

#define SCL_MISO_PIN		GPIO_Pin_6 // SPI MISO -> RX
#define SCL_MISO_SOURCE		GPIO_PinSource6

#define SCL_SPI                SPI1
#define SCL_SPI_CLK            RCC_APB2Periph_SPI1
#define SCL_SPI_CLK_INIT       RCC_APB2PeriphClockCmd
#define SCL_SPI_IRQ            SPI1_IRQn
#define SCL_SPI_IRQHANDEL      SPI1_IRQHandler
#define SCL_SPI_AF             GPIO_AF_SPI1


/* Private variables ---------------------------------------------------------*/

/* Public functions ---------------------------------------------------------*/
void SCL_SPI_IRQHANDLER(void);
void scl_init(uint8_t mode);
void scl_send_char(char data);
void scl_send_string(char *data);
char scl_receive_char(void);
char * scl_receive_string(void);


/* Private functions ---------------------------------------------------------*/
