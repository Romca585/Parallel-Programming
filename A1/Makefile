CFLAGS = -Wall
CC = gcc
LDFLAGS = -lpthread

all: gol_data gol_task

gol_data: gol_data.o
		$(CC) $(CFLAGS) -o gol_data gol_data.o $(LDFLAGS)

gol_data.o: gol_data.c
		$(CC) $(CFLAGS) -c gol_data.c $(LDFLAGS)

gol_task: gol_task.o
		$(CC) $(CFLAGS) -o gol_task gol_task.o $(LDFLAGS)

gol_task.o: gol_task.c
		$(CC) $(CFLAGS) -c gol_task.c $(LDFLAGS)


clean:
		rm gol_data.o gol_task.o