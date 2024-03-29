/*
 * vcs3i2c.h
 * https://www.kernel.org/doc/Documentation/i2c/dev-interface
 */

#ifndef VCS3I2C_H_
#define VCS3I2C_H_



#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <fcntl.h>    /* For O_RDWR */
#include <unistd.h>   /* For open(), */
#include <sys/ioctl.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define ADDR 0x63

#define  REG_READ_START    0x01
#define  REG_GET_DATA      0x09
#define  REG_READ_DATA     0x0D

#define  REG_READ_E25     0x02
#define  REG_READ_EC      0x03
#define  REG_READ_TEMP    0x04
#define  REG_READ_VWC     0x05
#define  REG_CAP     0x0A
#define  REG_RES     0x0B
#define  REG_RC     0x0C
#define  REG_RT     0x0D

#define  REG_CALIBRATE_AIR    0x06
#define  REG_CALIBRATE_WATER  0x07
#define  REG_CALIBRATE_EC  0x10
#define REG_CEC_GET 0x11

#define REG_SET_I2C_ADDR    0x08

int init(int address);
int newAddress(int newAddr);
int calibrationAir();
int calibrationWater();
int calibrationEC(int16_t valueUs);
int newReading();
float getE25();
float getEC();
float getTemp();
float getVWC();
float getCalEc();
int getData(float retVal[]);
int getDataLeaf(float retVal[]);
int16_t getCap();
int16_t getRc();
int16_t getVal(uint8_t reg);
uint32_t getVal32(uint8_t reg);
#endif /* VCS3I2C_H_ */
