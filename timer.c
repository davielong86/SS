#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <time.h>
#include <signal.h>
#include <syslog.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include "locked.h"
#include "log.h"


int getTime() {

	int argc;
	char **argv;
	sleep(1);
	int value = 1;
	time_t now;
	struct tm newyear;
	double seconds;
	newyear = *localtime(&now);
	newyear.tm_hour = 13;
	newyear.tm_min = 42; 
	newyear.tm_sec = 0;
	time(&now);  /* get current time; same as: now = time(NULL)  */

	//seconds = difftime(now,mktime(&newyear));
	seconds = 0;
	sleep(1);
	if (seconds  == 0) {

		int lockedValue = lock();


		char backup[100] = "/home/davie/Documents/SS1V2/Backup/www/";
		char toBackUp[100] = "/home/davie/Documents/SS1V2/Intranet/www";
		char str_timestamp[80];

		strftime(str_timestamp, 80, "%B_%d_%H", &newyear);
		strcat(backup, str_timestamp);

		char *command = "/bin/cp";
		char *arguments[] = { "cp", "-R", toBackUp, backup, NULL };

		int unlockedValue = unlock();


		if(lockedValue == 1) {
			logFile("lock", "Success");
		} else {
			logFile("lock", "Failure");
		}

		if(unlockedValue == 1) {
			logFile("Transfer", "Success");
			logFile("Unlock", "Success");
			execvp(command, arguments);
		} else {
			logFile("Unlock", "Failure");
		}
	}
	return value;
}
