###############################################################################
#
#  Sample Makefile
#    Template for E4E OpenCV applications
#    Written by Dustin Mendoza
#    modified by Angela To
#    and by Traci Takasugi
#    and by Luke Deluccia
#
#  Version No.
#    1.0
#
#  Created On:
#    7/1/2014
#
#  Edited On:
#
#
###############################################################################

GCC=g++
CFLAGS=`pkg-config --cflags opencv`
CLIBS=`pkg-config --libs opencv`
 
all: main
 
main: sample.o
        $(GCC) -o main sample.o $(CLIBS)
 
sample.o: sample.cpp
        $(GCC) $(CFLAGS) -c sample.cpp
 
clean:
        rm -rf *.o main