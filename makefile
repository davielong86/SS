CC=gcc
objects = main.o timer.o locked.o log.o find_updated.o sync.o
headers = timer.h locked.h log.h find_updated.h sync.h

myprog : $(objects)
	$(CC) -o prog $(objects) -lm

main.o : main.c $(headers)
	$(CC) -c main.c
	
timer.o : timer.c
	$(CC) -c timer.c

locked.o : locked.c
	$(CC) -c locked.c

log.o : log.c
	$(CC) -c log.c

find_updated.o : find_updated.c
	$(CC) -c find_updated.c

sync.o : sync.c
	$(CC) -c sync.c

clean: 
	rm prog main.o timer.o locked.o log.o find_updated.o sync.o

