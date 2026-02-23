#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "i2c.h"
#include "ssd1306.h"
#include "font.h"

static uint8_t global_x = 0;
static uint8_t global_y = 0;

/* ================= LOW LEVEL ================= */

static uint8_t oled_cmd(uint8_t cmd)
{
    uint8_t buf[2] = { SSD1306_COMM_CONTROL_BYTE, cmd };
    return _i2c_write(buf, 2);
}

static uint8_t oled_data_stream(uint8_t *data, uint16_t len)
{
    uint8_t buffer[1 + 128];

    if (len > 128)
        return 1;

    buffer[0] = SSD1306_DATA_CONTROL_BYTE;
    memcpy(&buffer[1], data, len);

    return _i2c_write(buffer, len + 1);
}

/* ================= INIT ================= */

int ssd1306_oled_init(void)
{
    if (_i2c_init(SSD1306_I2C_DEVICE, SSD1306_I2C_ADDR))
        return 1;

    sleep(1);

    oled_cmd(0xAE);
    oled_cmd(0xD5); oled_cmd(0x80);
    oled_cmd(0xA8); oled_cmd(0x3F);
    oled_cmd(0xD3); oled_cmd(0x00);
    oled_cmd(0x40);
    oled_cmd(0x8D); oled_cmd(0x14);
    oled_cmd(0x20); oled_cmd(0x02);
    oled_cmd(0xA1);
    oled_cmd(0xC8);
    oled_cmd(0xDA); oled_cmd(0x12);
    oled_cmd(0x81); oled_cmd(0x7F);
    oled_cmd(0xD9); oled_cmd(0xF1);
    oled_cmd(0xDB); oled_cmd(0x40);
    oled_cmd(0xA4);
    oled_cmd(0xA6);
    oled_cmd(0x2E);
    oled_cmd(0xAF);

    global_x = 0;
    global_y = 0;

    return ssd1306_oled_clear_screen();
}

/* ================= CURSOR ================= */

uint8_t ssd1306_oled_set_XY(uint8_t x, uint8_t y)
{
    if (x >= SSD1306_WIDTH || y >= SSD1306_PAGES)
        return 1;

    global_x = x;
    global_y = y;

    uint8_t buf[4] = {
        SSD1306_COMM_CONTROL_BYTE,
        (uint8_t)(0xB0 | y),
        (uint8_t)(0x00 | (x & 0x0F)),
        (uint8_t)(0x10 | (x >> 4))
    };

    return _i2c_write(buf, 4);
}

/* ================= CLEAR ================= */

uint8_t ssd1306_oled_clear_line(uint8_t row)
{
    if (row >= SSD1306_PAGES)
        return 1;

    ssd1306_oled_set_XY(0, row);

    uint8_t line[SSD1306_WIDTH];
    memset(line, 0x00, SSD1306_WIDTH);

    return oled_data_stream(line, SSD1306_WIDTH);
}

uint8_t ssd1306_oled_clear_screen(void)
{
    for (uint8_t i = 0; i < SSD1306_PAGES; i++)
        if (ssd1306_oled_clear_line(i))
            return 1;

    ssd1306_oled_set_XY(0, 0);
    return 0;
}

/* ================= TEXT ================= */

static void ssd1306_oled_write_char(uint8_t size, char c)
{
    const uint8_t *font_table;
    uint8_t width;

    if (size == SSD1306_FONT_SMALL)
    {
        font_table = font5x7;
        width = 5;
    }
    else
    {
        font_table = font8x8;
        width = 8;
    }

    if (c == '\n')
    {
        global_x = 0;
        global_y++;
        ssd1306_oled_set_XY(global_x, global_y);
        return;
    }

    if (c < 0x20 || c > 0x7E)
        c = '?';

    if (global_x + width >= SSD1306_WIDTH)
    {
        global_x = 0;
        global_y++;
        ssd1306_oled_set_XY(global_x, global_y);
    }

    const uint8_t *glyph = &font_table[(c - 0x20) * width];

    oled_data_stream((uint8_t*)glyph, width);

    global_x += width;
}

void ssd1306_oled_putc(uint8_t size, char c)
{
    ssd1306_oled_write_char(size, c);
}

void ssd1306_oled_puts(uint8_t size, const char* str)
{
    while (*str)
        ssd1306_oled_write_char(size, *str++);
}

/* ================= CONTROL ================= */

uint8_t ssd1306_oled_onoff(uint8_t onoff)
{
    return oled_cmd(onoff ? 0xAF : 0xAE);
}

uint8_t ssd1306_oled_set_contrast(uint8_t value)
{
    oled_cmd(0x81);
    return oled_cmd(value);
}
