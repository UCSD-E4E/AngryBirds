## Makefile for Angry Birds
# Created 07/15/2014
# E4E + REU

CXX = g++ -O2 -std=c++0x 
CXXFLAGS = 
CXXLIBS = -lpthread
BLACK_OBJS = BlackLib/BlackLib.o
SENSOR_SIGNAL_OBJS = SensorSignal/SensorSignal.o
OBJS= $(BLACK_OBJS) $(SENSOR_SIGNAL_OBJS)
all: $(OBJS) analog_stream test_main cron

test_main: test_main.o $(BLACK_OBJS) $(SENSOR_SIGNAL_OBJS)
	$(CXX) -o test_main test_main.o $(BLACK_OBJS) $(SENSOR_SIGNAL_OBJS) `pkg-config --libs opencv` $(CXXLIBS)
	mv test_main bin
analog_stream: analog_stream.cpp $(BLACK_OBJS)
	$(CXX) -o analog_stream analog_stream.cpp $(BLACK_OBJS)
	mv analog_stream bin
cron: cron/cron_test.cpp
	$(MAKE) -C cron

test_main.o: test_main.cpp
	$(CXX) -c test_main.cpp $(CXXLIBS)
BlackLib/BlackLib.o: BlackLib/BlackLib.cpp
	$(MAKE) -C BlackLib
SensorSignal/SensorSignal.o: SensorSignal/SensorSignal.cpp
	$(MAKE) -C SensorSignal

clean:
	rm -rf *.o analog_stream test_main
	rm -rf bin/test_main bin/analog_stream
	$(MAKE) -C BlackLib clean
	$(MAKE) -C cron clean
	$(MAKE) -C SensorSignal clean
