CC          = gcc
CCFLAGS     = -O3 -std=c99
INCFLAGS    = -I.
LDDFLAGS    = -lexpat

all: xmldump

xmldump: xmldump.c strstack.h
	$(CC) $(CCFLAGS) $(INCFLAGS) $(LDDFLAGS) -o $@ $<

clear:
	rm -vf xmldump

.PHONY: all clear
