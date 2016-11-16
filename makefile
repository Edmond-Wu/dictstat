CC = gcc
CFLAGS = -g -Wall -pedantic -ansi -std=c99
OBJS = dictstat.o
DEPS = dictstat.h

all: dictstat
	rm *.o

dictstat: $(OBJS)
	$(CC) $(CFLAGS) $^ -o dictstat

pointersorter.o: dictstat.c dictstat.h
	$(CC) $(CFLAGS) -c dictstat.c

clean:
	rm dictstat
