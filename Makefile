## Makefile for Angry Birds
# Created 07/15/2014
# E4E + REU

#To compile the normal program, type in "make main"

GCC = g++ -O2 -std=c++0x

all: main test testSig SensorSignal analog_stream test_main

main: main.o BlackLib.o SensorSignal.o
	$(GCC) -o main main.o BlackLib.o SensorSignal.o `pkg-config --libs opencv`
test_main: test_main.o BlackLib.o SensorSignal.o
	$(GCC) -o test_main test_main.o BlackLib.o SensorSignal.o `pkg-config --libs opencv`
analog_stream: analog_stream.cpp BlackLib.h BlackLib.cpp
	$(GCC) -o analog_stream analog_stream.cpp BlackLib.cpp
test: test.cpp 
	$(GCC) -o test test.cpp `pkg-config --libs opencv`
testSig: testSignal.cpp BlackLib.h BlackLib.cpp
	$(GCC) -o testSig testSignal.cpp BlackLib.cpp
SensorSignal: SensorSignal.cpp BlackLib.h BlackLib.cpp
	$(GCC) -o SensorSignal SensorSignal.cpp BlackLib.cpp

main.o: main.cpp
	$(GCC) -c main.cpp
test_main.o: test_main.cpp
	$(GCC) -c test_main.cpp
BlackLib.o: BlackLib.cpp
	$(GCC) -c BlackLib.cpp
SensorSignal.o: SensorSignal.cpp
	$(GCC) -c SensorSignal.cpp

clean:
	rm -rf *.o main test testSig SensorSignal analog_stream test_main
