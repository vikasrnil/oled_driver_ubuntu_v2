                                                                                                                                                                    
#include <stdio.h>
#include <unistd.h>

#include "i2c.h"
#include "ssd1306.h"
#include "functions.h"

int main()
{
        /* Initialize OLED */
    if (ssd1306_oled_init() != 0)
    {
        printf("OLED initialization failed!\n");
        return -1;
    }

    printf("OLED Ready\n");

    display_system_info();

    while (1)
    {
        sleep(5);
        display_system_info();
    }

    return 0;
}
