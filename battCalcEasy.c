#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define PATH_BATT_STATUS "/sys/class/power_supply/BAT0/status" //Dischrging etc
#define PATH_BATT_CAPACITY "/sys/class/power_supply/BAT0/capacity" //percents left

int main(int argc, char* argv[]) {

	char capacity[3];
	char status[20];

	float modifier = 4; //for time calcualtions

	// capacity file open
	int c;
	FILE *capacityFile;
	capacityFile = fopen(PATH_BATT_CAPACITY, "r");
	if (capacityFile) {
		int i = 0;
		while ((c = getc(capacityFile)) != EOF){
			//putchar(c); // output to console
			capacity[i] = c; //to string
			i++;
		}
		fclose(capacityFile);
	}

	// status file open
	int c1;
	FILE *statusFile;
	statusFile = fopen(PATH_BATT_STATUS, "r");
	if (statusFile) {
		int i = 0;
		while ((c1 = getc(statusFile)) != EOF){
			//putchar(c1); // output to console
			status[i] = c1; //to string
			i++;
		}
		fclose(statusFile);
	}

	int capacityValue = atoi(capacity); // string to int

	//calculations
	char chargingStr[] = "Charging";
	char dischargingStr[] = "Discharging";
	//printf("%d\n", strcmp(status,chargingStr));
	if (strcmp(status,chargingStr) == 10 || strcmp(status,chargingStr) == 0) {
		printf("Currently Charging\n");
	}
	else {
		int timeLeft;
		int hours;
		int mins;

		timeLeft = capacityValue * modifier; // in minutes
		hours = timeLeft / 60;
		mins = timeLeft % 60;

		printf("%d hours, %d minutes\n", hours, mins);
	}

	return 0;
}