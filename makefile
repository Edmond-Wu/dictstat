CC = gcc
CFLAGS = -g -Wall -pedantic -std=c99

dictstat : dictstat.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm dictstat
