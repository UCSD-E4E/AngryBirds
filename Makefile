## Makefile for Angry Birds
# Created 07/15/2014
# E4E + REU

CXX = g++ -O2 -std=c++0x 
CXXFLAGS = -g 
CXXLIBS = -lpthread
BLACK_OBJS = BlackLib/BlackLib.o
SENSOR_SIGNAL_OBJS = SensorSignal/SensorSignal.o
OBJS= $(BLACK_OBJS) $(SENSOR_SIGNAL_OBJS)
all: $(OBJS) analog_stream test_main cron

test_main: test_main.o $(BLACK_OBJS) $(SENSOR_SIGNAL_OBJS)
	$(CXX) -o test_main test_main.o $(BLACK_OBJS) $(SENSOR_SIGNAL_OBJS) `pkg-config --libs opencv` $(CXXLIBS)
	mv test_main bin
compress_main: compress_main.o $(BLACK_OBJS) $(SENSOR_SIGNAL_OBJS)
	$(CXX) -o compress_main compress_main.o $(BLACK_OBJS) $(SENSOR_SIGNAL_OBJS) `pkg-config --libs opencv` $(CXXLIBS)
ship_main: ship_main.o $(BLACK_OBJS) $(SENSOR_SIGNAL_OBJS)
	$(CXX) -o ship_main ship_main.o $(BLACK_OBJS) $(SENSOR_SIGNAL_OBJS) `pkg-config --libs opencv` $(CXXLIBS)
	mv ship_main bin
analog_stream: analog_stream.cpp $(BLACK_OBJS)
	$(CXX) -o analog_stream analog_stream.cpp $(BLACK_OBJS)
	mv analog_stream bin
cron: cron/cron_test.cpp
	$(MAKE) -C cron
queue_test: queue_test.o 
	$(CXX) -o queue_test queue_test.o `pkg-config --libs opencv` $(CXXLIBS)
	mv cron bin

test_main.o: test_main.cpp
	$(CXX) -c test_main.cpp $(CXXLIBS)
compress_main.o: compress_main.cpp
	$(CXX) -c compress_main.cpp $(CXXLIBS)
ship_main.o: ship_main.cpp
	$(CXX) -c ship_main.cpp $(CXXLIBS)
BlackLib/BlackLib.o: BlackLib/BlackLib.cpp
	$(MAKE) -C BlackLib
SensorSignal/SensorSignal.o: SensorSignal/SensorSignal.cpp
	$(MAKE) -C SensorSignal
queue_test.o: queue_test.cpp
	$(CXX) -c queue_test.cpp $(CXXLIBS)

clean:
	rm -rf *.o analog_stream test_main
	rm -rf bin/test_main bin/analog_stream
	$(MAKE) -C BlackLib clean
	$(MAKE) -C cron clean
	$(MAKE) -C SensorSignal clean
