CC          = gcc
CCFLAGS     = -O3 -std=c99
INCFLAGS    = -I.
LDDFLAGS    = -lexpat

all: xmldump

strstack.o: strstack.c strstack.h
	$(CC) $(CCFLAGS) $(INCFLAGS) $(LDDFLAGS) -c $<

xmldump: xmldump.c strstack.o
	$(CC) $(CCFLAGS) $(INCFLAGS) $(LDDFLAGS) -o $@ $^

clear:
	rm -vf strstack.o xmldump

.PHONY: all clear
