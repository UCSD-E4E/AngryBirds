## Makefile for Angry Birds
# Created 07/15/2014
# E4E + REU

#To compile the normal program, type in "make main"

CXX = g++ -O2 -std=c++0x 
CXXFLAGS = 
CXXLIBS = -lpthread
all: testSig analog_stream test_main

test_main: test_main.o BlackLib.o SensorSignal.o
	$(CXX) -o test_main test_main.o BlackLib.o SensorSignal.o `pkg-config --libs opencv` $(CXXLIBS)
analog_stream: analog_stream.cpp BlackLib.h BlackLib.cpp
	$(CXX) -o analog_stream analog_stream.cpp BlackLib.cpp
testSig: testSignal.cpp BlackLib.h BlackLib.cpp
	$(CXX) -o testSig testSignal.cpp BlackLib.cpp

test_main.o: test_main.cpp
	$(CXX) -c test_main.cpp $(CXXLIBS)
BlackLib.o: BlackLib.cpp
	$(CXX) -c BlackLib.cpp
SensorSignal.o: SensorSignal.cpp
	$(CXX) -c SensorSignal.cpp

clean:
	rm -rf *.o test testSig SensorSignal analog_stream test_main
