CXX = g++
CXXFLAGS = -O2 -std=c++0x
CXXLIBS = 
BLACK_OBJS = /home/ubuntu/AngryBirds/BlackLib/BlackLib.o


all: $(BLACK_OBJS) process_remote_signal led_test

process_remote_signal: process_remote_signal.cpp $(BLACK_OBJS)
	$(CXX) -o process_remote_signal process_remote_signal.cpp $(BLACK_OBJS)
led_test: led_test.cpp $(BLACK_OBJS)
	$(CXX) -o led_test led_test.cpp $(BLACK_OBJS)

clean:
	rm -rf process_remote_signal led_test
