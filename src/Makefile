CFLAGS=-g -Wall
INCLUDE=-I.
BUILDDIR=../build
MCU_TARGET=
CC=/usr/bin/gcc

%.o: %.c 
	$(CC) $(CFLAGS) $(INCLUDE) -o $(BUILDDIR)/$(MCU_TARGET)/$@ -c $<


all: ringbuffer.o

