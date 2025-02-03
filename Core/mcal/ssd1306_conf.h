/**
 * Private configuration file for the SSD1306 library.
 * This example is configured for STM32F0, I2C and including all fonts.
 */

#ifndef __SSD1306_CONF_H__
#define __SSD1306_CONF_H__


// Choose a bus
#define SSD1306_USE_I2C

#define SSD1306_I2C_PORT        hi2c4
#define SSD1306_I2C_ADDR        (0x3C << 1)


#define SSD1306_INCLUDE_FONT_6x8
#define SSD1306_INCLUDE_FONT_7x10
#define SSD1306_INCLUDE_FONT_11x18
#define SSD1306_INCLUDE_FONT_16x26

#define SSD1306_INCLUDE_FONT_16x24

#define SSD1306_INCLUDE_FONT_16x15

 #define SSD1306_WIDTH           128

 #define SSD1306_HEIGHT          32

#endif
