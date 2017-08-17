PROJECTNAME=ringbuffer
LIBNAME=lib$(PROJECTNAME)
TESTBINNAME=test_$(LIBNAME)

BASEDIR=$(CURDIR)
BINDIR=$(BASEDIR)/bin
BUILDDIR=$(BASEDIR)/build

INCLUDE=-I$(BASEDIR)/src
LIBS=
CFLAGS=-g -Wall

MAKE=/usr/bin/make
CC=/usr/bin/gcc

OBJECTS=$(PROJECTNAME).o

export MAKE BASEDIR BUILDDIR INCLUDE

.ONESHELL:

avrlib: CC = /usr/bin/avr-gcc
avrlib: MCU_TARGET = atmega328p
avrlib:
	export CC CFLAGS MCU_TARGET INCLUDE
	mkdir -p $(BUILDDIR)/$(MCU_TARGET)
	$(MAKE) -C src MCU_TARGET=$(MCU_TARGET)  all

lib:
	mkdir -p $(BUILDDIR)
	export MAKE CC CFLAGS INCLUDE
	$(MAKE) -C src all
	
all: clean avrlib

clean:
	rm -Rf $(BUILDDIR)/*
	rm -Rf $(BINDIR)/*

test: lib
	export MAKE CC CFLAGS INCLUDE
	make -C test all
	mkdir -p $(BINDIR)
	gcc -o $(BINDIR)/$(TESTBINNAME) $(BUILDDIR)/*.o
	$(BINDIR)/$(TESTBINNAME)
