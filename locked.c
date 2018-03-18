// Orphan Example
// The child process is adopted by init process, when parent process dies.
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <syslog.h>
#include <sys/stat.h>
#include <time.h>

int lock()
{
	int value = 1;
	char mode[] = "1111";
	char buf[100] = "/home/davie/Documents/SS1V2/Intranet/";
	int i;
	i = strtol(mode, 0, 8);
	chmod(buf, i);
	return value;
}

int unlock() {
	sleep(5);
	int value = 1;
	char mode[] = "07777";
	char buf[100] = "/home/davie/Documents/SS1V2/Intranet/";
	int i;
	i = strtol(mode, 0, 8);
	chmod(buf, i);
	return value;
}