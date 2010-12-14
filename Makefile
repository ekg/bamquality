
# Generic settings
SHELL = /bin/sh

# Targets
MYBINS = bamgrouper

# Building binaries
CC = gcc
CXX = g++
CFLAGS = -Wall -O3
CXXFLAGS = -Wall -O3
#LDENV = -Llib/
#LDFLAGS = -lbamtools
LIBS = -lz -lm

OBJECTS=BamAlignment.o \
		BamMultiReader.o \
		BamReader.o \
		BamIndex.o \
		BamWriter.o \
		BGZF.o

all: bamquality

clean:
	rm *.o

.PHONY: all clean

bamquality: bamquality.cpp $(OBJECTS)
	$(LDENV) $(CXX) $(CXXFLAGS) bamquality.cpp -o $@ $(OBJECTS) $(LDFLAGS) $(LIBS)


