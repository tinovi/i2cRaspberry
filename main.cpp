#include <linux/i2c-dev.h>
#include <errno.h>


#define I2C_ADDR 0x63


int main (void) {
    int value;
    int fd;

    fd = open("/dev/i2c-1", O_RDWR);
    if (fd < 0) {
        printf("Error opening file: %s\n", strerror(errno));
        return 1;
    }
    if (ioctl(fd, I2C_SLAVE, I2C_ADDR) < 0) {
        printf("ioctl error: %s\n", strerror(errno));
        return 1;
    }

    return 0;
}
