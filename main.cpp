#include "vcs3i2c.h"
#include <ncurses.h>
#define I2C_ADDR 0x63


int main (void) {
	init(ADDR);
	newReading(); // start sensor reading
	napms(30); //let sensor read data
	float e25 = getE25();
	float ec = getEC();
	float temp = getTemp();
	float vwc = getVWC();
    return 0;
}
