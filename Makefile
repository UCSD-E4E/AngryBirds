## Makefile for Angry Birds
# Created 07/15/2014
# E4E + REU

GCC = g++ -O2 -std=c++0x

CAM_LIB = sample.cpp BlackLib.cpp

sample.o: sample.cpp
	$(GCC) -c sample.cpp
BlackLib.o: BlackLib.cpp
	$(GCC) -c BlackLib.cpp
main: sample.o BlackLib.o
	$(GCC) -o main sample.o BlackLib.o `pkg-config --libs opencv`
test: test.cpp 
	$(GCC) -o test test.cpp `pkg-config --libs opencv`
	
