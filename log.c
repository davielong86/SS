#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>


void logFile(char message[], char status[]){
	FILE *fp;
	fp = fopen("/home/davie/Documents/SS1V2/log.txt","a");
	
	time_t now;
	time(&now);/* get current time; same as: now = time(NULL)  */ 
	
	char *buf;
	buf=(char *)malloc(10*sizeof(char));
	buf=getlogin(); // get user logged in
	printf("\n %s \n",buf);
	
	if(fp == NULL) {
		printf("Error opening file");
		exit(1);
	}

	fprintf(fp,"\n-Changes Made to file-");
	fprintf(fp,"Time    : %s\n",ctime(&now));
	fprintf(fp,"Status  : %s\n",status);
	fprintf(fp,"Message : %s\n",message);
	fprintf(fp,"User    : %s\n",buf);
	fprintf(fp,"\n--------------------------------------------------------------");
	fclose(fp);
}