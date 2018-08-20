#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include "vcs3i2c.h"

int main(int argc, char **argv) {

	if (argc < 3) //Check enough arguments we're supplied
			{
		fprintf(stderr,
				"require arguments: <address> <command> <... optional command args> \n");
		exit (EXIT_FAILURE);
	}
	if (strncmp(argv[2], "help", 100) == 0) {
		fprintf(stdout, "usage: \n");
		fprintf(stdout, "read vals: <Address> vals \n");
		fprintf(stdout, "change address: <old Address> addr <new Address> \n");
		return 0;
	}
	int addr = strtol(argv[1], NULL, 16);
	init(addr);
	if (strncmp(argv[2], "addr", 100) == 0) {
		if (argc < 4) {
			fprintf(stderr, "Please supply new addres\n");
			exit (EXIT_FAILURE);
		}
		int i = strtol(argv[3], NULL, 16);
		newAddress(i);
		return 0;
	}

	if (strncmp(argv[2], "vals", 100) == 0) {
		newReading(); // start sensor reading
		napms(30); //let sensor read data
		float e25 = getE25();
		float ec = getEC();
		float temp = getTemp();
		float vwc = getVWC();
		fprintf(stdout,"%.2f,%.2f,%.2f,%.2f\r\n",e25, ec,temp,vwc);
		return 0;
	}

	if (strncmp(argv[2], "air", 100) == 0) {
		fprintf(stdout, "calibration result: %i",calibrationAir());
		return 0;
	}
	if (strncmp(argv[2], "air", 100) == 0) {
		fprintf(stdout, "calibration result: %i",calibrationAir());
		return 0;
	}
	if (strncmp(argv[2], "water", 100) == 0) {
		fprintf(stdout, "calibration result: %i",calibrationWater());
		return 0;
	}
	return 0;
}
