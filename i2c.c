#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include "i2c.h"

static int file_i2c = -1;  

uint8_t _i2c_init(const char *device, int dev_addr)
{
    if (device == NULL)
        return 1;

    // If already open, close first
    if (file_i2c >= 0)
    {
        close(file_i2c);
        file_i2c = -1;
    }

    file_i2c = open(device, O_RDWR);
    if (file_i2c < 0)
    {
        perror("I2C: Failed to open device");
        return 1;
    }

    if (ioctl(file_i2c, I2C_SLAVE, dev_addr) < 0)
    {
        perror("I2C: Failed to set slave address");
        close(file_i2c);
        file_i2c = -1;
        return 1;
    }

    return 0;
}

uint8_t _i2c_close(void)
{
    if (file_i2c >= 0)
    {
        close(file_i2c);
        file_i2c = -1;
        return 0;
    }

    return 1;
}

uint8_t _i2c_write(const uint8_t *ptr, int16_t len)
{
    if (file_i2c < 0 || ptr == NULL || len <= 0)
        return 1;

    ssize_t ret = write(file_i2c, ptr, len);

    if (ret != len)
    {
        perror("I2C: Write failed");
        return 1;
    }

    return 0;
}

uint8_t _i2c_read(uint8_t *ptr, int16_t len)
{
    if (file_i2c < 0 || ptr == NULL || len <= 0)
        return 1;

    ssize_t ret = read(file_i2c, ptr, len);

    if (ret != len)
    {
        perror("I2C: Read failed");
        return 1;
    }

    return 0;
}
