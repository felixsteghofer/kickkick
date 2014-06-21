CC=gcc
CFLAGS=-lwiringPi

all: oKicker.c
	$(CC) -o oKicker oKicker.c $(CFLAGS)
