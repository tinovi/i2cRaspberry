/*
 * vcs3i2c.cpp
 *
 */

#include "vcs3i2c.h"
#include <unistd.h>

int addr = 0x63;
int fd;

int init(int address) {
	addr = address;
	fd = open("/dev/i2c-1", O_RDWR);
	if (fd < 0) {
		fprintf(stderr,"Error opening file: %s\n", strerror(errno));
		return -1;
	}
	if (ioctl(fd, I2C_SLAVE, addr) < 0) {
		fprintf(stderr,"ioctl error: %s\n", strerror(errno));
		return -1;
	}

	return 0;
}

uint8_t readB[8] = { 0 };
uint8_t weriteB[3] = { 0 };
int length;

int getState() { //-1:no data, 0:err, 1:ok
	usleep(300000);
	length = 1;			//<<< Number of bytes to read
	if (read(fd, readB, length) != length) {
		//ERROR HANDLING: i2c transaction failed
		fprintf(stderr,"getState::Failed to read.\n");
		return -1;
	} else {
		//printf("Data read: %s\n", readB);
		return readB[0];
	}
}

int16_t getVal(uint8_t reg) {

	weriteB[0] = reg;
	length = 1;			//<<< Number of bytes to write
	if (write(fd, weriteB, length) != length)
			{
		/* ERROR HANDLING: i2c transaction failed */
		fprintf(stderr,"getVal:%i:Failed to write.\n",reg);
		return -1;
	}
	usleep(10000);
	length = 2;			//<<< Number of bytes to read
	if (read(fd, readB, length) != length) {
		//ERROR HANDLING: i2c transaction failed
		fprintf(stderr,"getVal:%i:Failed to read .\n");
		return -1;
	} else {
		//printf("Data read: %s\n", readB);
		int16_t ret;
		uint8_t *pointer = (uint8_t *) &ret;
		pointer[0] = readB[0];
		pointer[1] = readB[1];
		return ret;
	}
}

uint32_t getVal32(uint8_t reg) {

        weriteB[0] = reg;
        length = 1;                     //<<< Number of bytes to write
        if (write(fd, weriteB, length) != length)
                        {
                /* ERROR HANDLING: i2c transaction failed */
                fprintf(stderr,"getVal32:%i:Failed to write.\n",reg);
                return -1;
        }
        usleep(10000);
        length = 4;                     //<<< Number of bytes to read
        if (read(fd, readB, length) != length) {
                //ERROR HANDLING: i2c transaction failed
                fprintf(stderr,"getVal32:%i:Failed to read .\n");
                return -1;
        } else {
                //printf("Data read: %s\n", readB);
                uint32_t ret;
                uint8_t *pointer = (uint8_t *) &ret;
                pointer[0] = readB[0];
                pointer[1] = readB[1];
                pointer[2] = readB[2];
                pointer[3] = readB[3];

                return ret;
        }
}


int setReg8(uint8_t reg, uint8_t val) {
	weriteB[0] = reg;
	weriteB[1] = val;
	length = 2;			//<<< Number of bytes to write
	if (write(fd, weriteB, length) != length)
			{
		/* ERROR HANDLING: i2c transaction failed */
		fprintf(stderr,"Failed to write to the i2c bus.\n");
		return -1;
	} else {
		return getState();
	}
}

int setReg(uint8_t reg, long sleep) {
	weriteB[0] = reg;
	length = 1;			//<<< Number of bytes to write
	if (write(fd, weriteB, length) != length)
			{
		/* ERROR HANDLING: i2c transaction failed */
		fprintf(stderr,"setReg:%i:Failed to write.\n",reg);
		return -1;
	} else {
		usleep(sleep);
		return getState();
	}
}

int calibrationEC(int16_t valueUs)
{
	fprintf(stderr,"calibrationEC:%i.\n",valueUs);
	weriteB[0] = REG_CALIBRATE_EC;
	uint8_t *pointer = (uint8_t *)&valueUs;
	weriteB[1] = pointer[0];
	weriteB[2] = pointer[1];
	length = 3;			//<<< Number of bytes to write
	if (write(fd, weriteB, length) != length)
			{
		/* ERROR HANDLING: i2c transaction failed */
		fprintf(stderr,"Failed to write to the i2c bus.\n");
		return -1;
	} else {
		return getState();
	}
}

int calibrationAir() {
	return setReg(REG_CALIBRATE_AIR,300000);
}

int calibrationWater() {
	return setReg(REG_CALIBRATE_WATER,300000);
}

int newAddress(int newAddr) {
	return setReg8(REG_SET_I2C_ADDR, newAddr);
}

int newReading() {
	return setReg(REG_READ_START,300000);
}

float getE25() {
	return getVal(REG_READ_E25) / 100.0;
}

int16_t getCap(){
    return getVal(REG_CAP);
}

int16_t getRc(){
    return getVal(REG_RC);
}


float getEC() {
	return getVal(REG_READ_EC) / 10.0;
}

float getTemp() {
	return getVal(REG_READ_TEMP) / 100.0;
}

float getVWC() {
	return getVal(REG_READ_VWC)/ 10.0;
}

float getCalEc(){
	return getVal(REG_CEC_GET) / 100.0;
}

int getData(float readings[]) {
	weriteB[0] = REG_GET_DATA;
	length = 1;			//<<< Number of bytes to write
	if (write(fd, weriteB, length) != length)
			{
		/* ERROR HANDLING: i2c transaction failed */
		fprintf(stderr,"getData::Failed to write \n");
		return -1;
	} else {

		length = 8;			//<<< Number of bytes to read
		if (read(fd, readB, length) != length) {
			//ERROR HANDLING: i2c transaction failed
			fprintf(stderr,"getData::Failed to read \n");
			return -1;
		} else {
			int16_t ret;
			for (int ar = 0; ar < 4; ar++) {
				uint8_t *pointer = (uint8_t *) &ret;
				pointer[0] = readB[ar * 2];
				pointer[1] = readB[ar * 2 + 1];
				switch(ar){
					case 0:
						readings[ar] = ret / 100.0;
						break;
					case 1:
						readings[ar] = ret / 10.0;
						break;
					case 2:
						readings[ar] = ret / 100.0;
						break;
					case 3:
						readings[ar] = ret / 10.0;
						break;
				}
			}

		}
	}
	return 0;
}

int getDataLeaf(float readings[]) {
	weriteB[0] = REG_GET_DATA;
	length = 1;			//<<< Number of bytes to write
	if (write(fd, weriteB, length) != length)
			{
		/* ERROR HANDLING: i2c transaction failed */
		fprintf(stderr,"getData::Failed to write \n");
		return -1;
	} else {

		length = 4;			//<<< Number of bytes to read
		if (read(fd, readB, length) != length) {
			//ERROR HANDLING: i2c transaction failed
			fprintf(stderr,"getData::Failed to read \n");
			return -1;
		} else {
			int16_t ret;
			for (int ar = 0; ar < 2; ar++) {
				uint8_t *pointer = (uint8_t *) &ret;
				pointer[0] = readB[ar * 2];
				pointer[1] = readB[ar * 2 + 1];
				readings[ar] = ret / 100.0;
			}

		}
	}
	return 0;
}
