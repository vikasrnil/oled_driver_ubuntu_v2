#include <stdio.h>
#include <unistd.h>

#include "ssd1306.h"

int main(void)
{
    printf("Initializing OLED...\n");

    /* Initialize OLED (includes I2C init internally) */
    if (ssd1306_oled_init() != 0)
    {
        printf("OLED initialization failed!\n");
        return -1;
    }

    printf("OLED initialized successfully.\n");

    /* Clear screen */
    ssd1306_oled_clear_screen();

    /* Set cursor to first page */
    ssd1306_oled_set_XY(0, 0);

    /* Write text */
    ssd1306_oled_puts(SSD1306_FONT_NORMAL,
        "Hello my name is\n"
        "Escorts Kubota Ltd\n"
        "SSD1306 Test\n");

    /* Keep running */
    while (1)
    {
        sleep(1);
    }

    return 0;
}
