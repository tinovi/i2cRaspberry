//#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vcs3i2c.h"
#include <unistd.h>

int main(int argc, char **argv) {

	if (argc < 3) //Check enough arguments we're supplied
			{
		fprintf(stderr,
				"require arguments: <address> <command> <... optional command args> \n");
		exit (EXIT_FAILURE);
	}
	if (strcmp(argv[2], "help") == 0) {
		fprintf(stdout, "usage: \n");
		fprintf(stdout, "read vals: <Address> vals \n");
		fprintf(stdout, "change address: <old Address> addr <new Address> \n");
		fprintf(stdout, "calibrate air: <Address> air \n");
		fprintf(stdout, "calibrate water: <Address> water \n");
		fprintf(stdout, "calibrate ec: <Address> calec <current µS/cm> \n");
		return 0;
	}
	int addr = strtol(argv[1], NULL, 16);
	init(addr);
	if (strcmp(argv[2], "addr") == 0) {
		if (argc < 4) {
			fprintf(stderr, "Please supply new addres\n");
			exit (EXIT_FAILURE);
		}
		int i = strtol(argv[3], NULL, 16);
		newAddress(i);
		return 0;
	}

	if (strcmp(argv[2], "calec") == 0) {
		if (argc < 4) {
			fprintf(stderr, "Please supply new value in µS/cm\n");
			exit (EXIT_FAILURE);
		}
		int i = strtol(argv[3], NULL, 16);
		calibrationEC(i);
		return 0;
	}

	if (strcmp(argv[2], "vals") == 0) {
		newReading(); // start sensor reading
		usleep(200000); //let sensor read data
		//fprintf(stdout,"%.2f,%.2f,%.2f,%.2f\r\n",getE25(), getEC(),getTemp(),getVWC());
		float fat[4];
		getData(fat);
		fprintf(stdout,"%.2f,%.2f,%.2f,%.2f\r\n",fat[0], fat[1],fat[2],fat[3]);
		return 0;
	}

	if (strcmp(argv[2], "air") == 0) {
		fprintf(stdout, "calibration result: %i",calibrationAir());
		return 0;
	}
	if (strcmp(argv[2], "water") == 0) {
		fprintf(stdout, "calibration result: %i",calibrationWater());
		return 0;
	}
	return 0;
}
