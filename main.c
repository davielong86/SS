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
#include "timer.h"
#include "locked.h"
#include "find_updated.h"
#include "sync.h"

#define LEN 150

int main(){

        // Create a child process      
        int pid = fork();
        int pid2 = fork();
        int ppid;
        int ppid2;
        FILE *pFile;
        char buffer[256];
        ppid = getpid();
        ppid2 = getpid();

        if (pid > 0) {
            printf("Starting Process\nCreating Parent and Child \n");
            // if PID > 0 :: this is the parent
            // this process performs printf and finishes
            //sleep(15);  // uncomment to wait 15 seconds before process ends
            exit(EXIT_SUCCESS);
        } 
        else if (pid == 0) {
            // Step 1: Create the orphan process
            printf("Child process is now an orphan process\n");

            // Step 2: Elevate the orphan process to session leader, to loose controlling TTY
            // This command runs the process in a new session
            if (setsid() < 0) { exit(EXIT_FAILURE); }

            int pid = fork();
            if (pid > 0) {
                exit(EXIT_SUCCESS);
            } 
            else {
                printf("Daemon process created and running\n");
                // Step 3: call umask() to set the file mode creation mask to 0
                // This will allow the daemon to read and write files 
                // with the permissions/access required 
                umask(0);

                // Step 4: Change the current working dir to root.
                // This will eliminate any issues of running on a mounted drive, 
                // that potentially could be removed etc..
                if (chdir("/") < 0 ) { 
                    printf("failed");
                    exit(EXIT_FAILURE); 
                }

                // Step 5: Close all open file descriptors
                /* Close all open file descriptors */
                int x;
                for (x = sysconf(_SC_OPEN_MAX); x>=0; x--)
                {
                    close (x);
                } 
                while(1){
                    int val = getTime();
                    if (val == 1){
                        syncSite();
                        find_updated();
                    }
                }
            }
            
        }
    return 0;
}