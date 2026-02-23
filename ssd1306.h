#ifndef __SSD1306_H__
#define __SSD1306_H__

#include <stdint.h>

#define SSD1306_I2C_DEVICE "/dev/i2c-1"
#define SSD1306_I2C_ADDR   0x3C

#define SSD1306_WIDTH      128
#define SSD1306_HEIGHT     64
#define SSD1306_PAGES      (SSD1306_HEIGHT/8)

/* Control bytes */
#define SSD1306_COMM_CONTROL_BYTE   0x00
#define SSD1306_DATA_CONTROL_BYTE   0x40

/* Font types */
#define SSD1306_FONT_SMALL   0
#define SSD1306_FONT_NORMAL  1

int  ssd1306_oled_init(void);
uint8_t ssd1306_oled_set_XY(uint8_t x, uint8_t y);
uint8_t ssd1306_oled_clear_screen(void);
uint8_t ssd1306_oled_clear_line(uint8_t row);

void ssd1306_oled_putc(uint8_t size, char c);
void ssd1306_oled_puts(uint8_t size, const char* str);

uint8_t ssd1306_oled_onoff(uint8_t onoff);
uint8_t ssd1306_oled_set_contrast(uint8_t value);

#endif
