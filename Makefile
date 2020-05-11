# 
# Makefile : LEOSoC Makefile Example
# 
# Description: 
# This Makefile generates the LEOSoC Application binary for the ARM CortexA9
#  
# Rev. 0.1
# 
# Author: Andrea Guerrieri <andrea.guerrieri@epfl.ch (C) 2019
#
# Copyright: See COPYING file that comes with this distribution
#
# 

CROSS_COMPILE ?=

AS		= $(CROSS_COMPILE)as
CC		= $(CROSS_COMPILE)g++
LD		= $(CC) -nostdlib
CPP		= $(CC) -E
AR		= $(CROSS_COMPILE)ar -q
NM		= $(CROSS_COMPILE)nm
STRIP		= $(CROSS_COMPILE)strip
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump
AWK		= awk
GENKSYMS	= scripts/genksyms/genksyms
DEPMOD		= /sbin/depmod
KALLSYMS	= scripts/kallsyms
PERL		= perl
CHECK		= sparse

APP = testapplication

SRCDIR=./examples
OBJDIR=./LEOSoC
BINDIR=./bin
LEOSoCLIB=./LEOSoC

CFLAGS=-O0 -g -Wall -mcpu=cortex-a9  -mfloat-abi=hard -fpermissive

#CFLAGS=-O0 -g -Wall -fpermissive

LFLAGS=-lpthread -lm -static-libgcc -static

IDIR=$(LEOSoCLIB)


$(BINDIR)/$(APP) :: $(LEOSoCLIB)/LEOSoC.o $(LEOSoCLIB)/amu.o $(LEOSoCLIB)/pmu.o $(LEOSoCLIB)/fpga_if.o $(SRCDIR)/testapplication.o
	$(CC) $? -o $@ $(LDIR) $(LFLAGS)

$(LEOSoCLIB)/LEOSoC.o :: $(LEOSoCLIB)/LEOSoC.cpp
	$(CC) $(LFLAGS) $(CFLAGS) -c $? -o $@ -I $(IDIR) -I $(SRCDIR)

$(LEOSoCLIB)/pmu.o :: $(LEOSoCLIB)/pmu.cpp
	$(CC) $(LFLAGS) $(CFLAGS) -c $? -o $@ -I $(IDIR) -I $(SRCDIR)

$(LEOSoCLIB)/amu.o :: $(LEOSoCLIB)/amu.cpp
	$(CC) $(LFLAGS) $(CFLAGS) -c $? -o $@ -I $(IDIR) -I $(SRCDIR)

$(LEOSoCLIB)/fpga_if.o :: $(LEOSoCLIB)/fpga_if.cpp
	$(CC) $(LFLAGS) $(CFLAGS) -c $? -o $@ -I $(IDIR) -I $(SRCDIR)

$(SRCDIR)/testapplication.o :: $(SRCDIR)/testapplication.cpp
	$(CC) $(LFLAGS) $(CFLAGS) -c $? -o $@ -I $(IDIR) -I $(SRCDIR)

clean ::
	rm -rf $(OBJDIR)/*.o $(SRCDIR)/*.o 
