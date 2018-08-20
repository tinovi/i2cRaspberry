/*
 * vcs3i2c.cpp
 *
 */

#include "vcs3i2c.h"
#include <linux/i2c-dev.h>
#include <errno.h>

int addr = 0x63;
int fd;

int init(int address) {
	addr = address;
	fd = open("/dev/i2c-1", O_RDWR);
	if (fd < 0) {
		printf("Error opening file: %s\n", strerror(errno));
		return -1;
	}
	if (ioctl(fd, I2C_SLAVE, addr) < 0) {
		printf("ioctl error: %s\n", strerror(errno));
		return -1;
	}

	return 0;
}

uint8_t readB[8] = { 0 };
uint8_t weriteB[2] = { 0 };
int length;

int getState() { //-1:no data, 0:err, 1:ok
	length = 1;			//<<< Number of bytes to read
	if (read(fd, readB, length) != length) {
		//ERROR HANDLING: i2c transaction failed
		printf("Failed to read from the i2c bus.\n");
		return -1;
	} else {
		//printf("Data read: %s\n", readB);
		return readB[0];
	}
}

int16_t getVal(uint8_t reg) {

	weriteB[0] = reg;
	length = 1;			//<<< Number of bytes to write
	if (write(fd, weriteB, length) != length)//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
			{
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");
		return -1;
	}
	length = 2;			//<<< Number of bytes to read
	if (read(fd, readB, length) != length) {
		//ERROR HANDLING: i2c transaction failed
		printf("Failed to read from the i2c bus.\n");
		return -1;
	} else {
		//printf("Data read: %s\n", readB);
		int16_t ret;
		byte *pointer = (uint8_t *) &ret;
		pointer[0] = readB[0];
		pointer[1] = readB[1];
		return ret;
	}
}

int setReg8(uint8_t reg, uint8_t val) {
	weriteB[0] = reg;
	weriteB[0] = val;
	length = 2;			//<<< Number of bytes to write
	if (write(fd, weriteB, length) != length)//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
			{
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");
		return -1;
	} else {
		return getState();
	}
}

int setReg(byte reg) {
	weriteB[0] = reg;
	length = 1;			//<<< Number of bytes to write
	if (write(fd, weriteB, length) != length)//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
			{
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");
		return -1;
	} else {
		return getState();
	}
}

int calibrationAir() {
	return setReg(REG_CALIBRATE_AIR);
}

int calibrationWater() {
	return setReg(REG_CALIBRATE_WATER);
}

int newAddress(int newAddr) {
	return setReg8(REG_SET_I2C_ADDR, newAddr);
}

int newReading() {
	return setReg(REG_READ_START);
}

float getE25() {
	return getVal(REG_READ_E25) / 100;
}

float getEC() {
	return getVal(REG_READ_EC) / 100;
}

float getTemp() {
	return getVal(REG_READ_TEMP) / 100;
}

float getVWC() {
	return getVal(REG_READ_VWC) / 100;
}

int getData(float *readings) {
	weriteB[0] = REG_GET_DATA;
	length = 1;			//<<< Number of bytes to write
	if (write(fd, weriteB, length) != length)//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
			{
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");
		return -1;
	} else {

		length = 8;			//<<< Number of bytes to read
		if (read(fd, readB, length) != length) {
			//ERROR HANDLING: i2c transaction failed
			printf("Failed to read from the i2c bus.\n");
			return -1;
		} else {
			int16_t ret;
			for (int ar = 0; ar < 4; ar++) {
				byte *pointer = (byte *) &ret;
				pointer[0] = readB[ar * 2];
				pointer[1] = readB[ar * 2 + 1];
				if (ar < 3) {
					readings[ar] = ret / 100;
				} else {
					readings[ar] = ret;
				}
			}
		}
	}
}
