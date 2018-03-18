#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <signal.h>

int pid;
int pipefd1[2];
int pipefd2[2];
int found_word = 0;

void update();

void main() {
  char data[4096];
  FILE *f = fopen("files.txt", "w");

  // Create pipe between find and awk
  pipe(pipefd1);

  // fork (find)
  pid = fork();

  if (pid == 0) {
    dup2(pipefd1[1], 1);

    // close fds
    close(pipefd1[0]);
    close(pipefd1[1]);

    execlp("find", "find", "/home/davie/Documents/SS1V2/Intranet/www", "-mtime", "-1", "-ls",  NULL);

    _exit(1);
  }

  // Create pipe between awk and sort
  pipe(pipefd2);

  // fork (awk)
  pid = fork();

  if (pid == 0) {
    // get the input from pipe 1
    dup2(pipefd1[0], 0);

    // set the output to pipe 2
    dup2(pipefd2[1], 1);

    // close fds
    close(pipefd1[0]);
    close(pipefd1[1]);
    close(pipefd2[0]);
    close(pipefd2[1]);

    // exec (awk), specifying user, date and filepath
    execlp("awk", "awk", "{print $11}", NULL);

    // error check
    perror("bad exec grep root");
    _exit(1);
  }

  // close unused fds
  close(pipefd1[0]);
  close(pipefd1[1]);

  // fork (sort)
  if (pid == 0) {
    // get the input from pipe 2
    dup2(pipefd2[0], 0);

    // close fds
    close(pipefd2[0]);
    close(pipefd2[1]);

    _exit(1);
  } else {
    close(pipefd2[1]);
    int nbytes = read(pipefd2[0], data, sizeof(data));
    //printf("%.*s", nbytes, data);
    fprintf(f, "%.*s", nbytes, data);
    close(pipefd2[0]);
    fclose(f);
  }

  update();
}


void update() { 
  
  int argc;
  char **argv;
  sleep(1);
  int value = 1;
  time_t now;
  struct tm newyear;
  double seconds;
  int files;

  time(&now);  /* get current time; same as: now = time(NULL)  */

  newyear = *localtime(&now);
  newyear.tm_hour = 11; 
  newyear.tm_min = 13; 
  newyear.tm_sec = 0;

  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;


  fp = fopen("files.txt", "r");

  int i = 0;
  while ((read = getline(&line, &len, fp)) != -1) {
      i = i + 1;
  }
  files = i;
  printf("Files is: %d\n", files);

  char paths[files][100];

  int j = 0;
  while ((read = getline(&line, &len, fp)) != -1) {
      paths[j][100] = line;
      j = j + 1;
  }
  fclose(fp);


  time(&now);
  seconds = difftime(now,mktime(&newyear));
  if (seconds  == 0) {

    files = files - 1;
    printf("(1)Files is: %d\n", files);
    
    for (int i = 0; i < files; i++) {
      printf("t");
      char LiveSite[100] = "/home/davie/Documents/SS1V2/liveSite/www";
      char str_timestamp[80];

      strftime(str_timestamp, 80, "%B_%d_%H", &newyear);
      
      strcat(paths[files], str_timestamp);
      
      char *command = "/bin/cp";
      char *arguments[] = { "cp", "-R", LiveSite, paths[files], NULL };
      execvp(command, arguments);
      files = files -1;
      printf("paths[files] is: %s\n", paths[files]);
      printf("Files is :%d\n", files);
    }

    }
}


