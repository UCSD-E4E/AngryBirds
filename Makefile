## Makefile for Angry Birds
# Created 07/15/2014
# E4E + REU

CXX = g++ -O2 -std=c++0x 
CXXFLAGS =  
CXXLIBS = -lpthread
BLACK_OBJS = BlackLib/BlackLib.o
SERVER_OBJS = Server/ServerSocket.o Socket/Socket.o
CLIENT_OBJS = Client/ClientSocket.o Socket/Socket.o
SENSOR_SIGNAL_OBJS = SensorSignal/SensorSignal.o
SERIAL_OBJS = serial/serialib.o
OBJS= $(BLACK_OBJS) $(SENSOR_SIGNAL_OBJS) $(SERIAL_OBJS)
all: $(OBJS) analog_stream cron sendStop ir_compress_main 

test_main: test_main.o $(BLACK_OBJS) $(SENSOR_SIGNAL_OBJS)
	$(CXX) -o test_main test_main.o $(BLACK_OBJS) $(SENSOR_SIGNAL_OBJS) `pkg-config --libs opencv` $(CXXLIBS)
	mv test_main bin
compress_main: compress_main.o $(BLACK_OBJS) $(SENSOR_SIGNAL_OBJS) $(SERVER_OBJS) $(SERIAL_OBJS)
	$(CXX) -o compress_main compress_main.o $(SERVER_OBJS) $(BLACK_OBJS) $(SENSOR_SIGNAL_OBJS) $(SERIAL_OBJS) `pkg-config --libs opencv` $(CXXLIBS)
	mv compress_main bin
ir_compress_main: ir_compress_main.o $(BLACK_OBJS) $(SENSOR_SIGNAL_OBJS) $(SERVER_OBJS) $(SERIAL_OBJS)
	$(CXX) -o ir_compress_main ir_compress_main.o $(SERVER_OBJS) $(BLACK_OBJS) $(SENSOR_SIGNAL_OBJS) $(SERIAL_OBJS) `pkg-config --libs opencv` $(CXXLIBS)
	mv ir_compress_main bin
ship_main: ship_main.o $(BLACK_OBJS) $(SENSOR_SIGNAL_OBJS)
	$(CXX) -o ship_main ship_main.o $(BLACK_OBJS) $(SENSOR_SIGNAL_OBJS) `pkg-config --libs opencv` $(CXXLIBS)
	mv ship_main bin
analog_stream: analog_stream.cpp $(BLACK_OBJS)
	$(CXX) -o analog_stream analog_stream.cpp $(BLACK_OBJS)
	mv analog_stream bin
cron: cron/cron_test.cpp
	$(MAKE) -C cron
sendStop: sendStop.o $(CLIENT_OBJS)
	$(CXX) -o sendStop sendStop.o $(CLIENT_OBJS)
	mv sendStop bin

serialib.o: serial/serialib.cpp
	$(CXX) -c serial/serialib.cpp $(CXXLIBS)
sendStop.o: sendStop.cpp
	$(CXX) -c sendStop.cpp $(CXXLIBS)
test_main.o: test_main.cpp
	$(CXX) -c test_main.cpp $(CXXLIBS)
compress_main.o: compress_main.cpp
	$(CXX) -c compress_main.cpp $(CXXLIBS)
ir_compress_main.o: ir_compress_main.cpp
	$(CXX) -c ir_compress_main.cpp $(CXXLIBS)
ship_main.o: ship_main.cpp
	$(CXX) -c ship_main.cpp $(CXXLIBS)
BlackLib/BlackLib.o: BlackLib/BlackLib.cpp
	$(MAKE) -C BlackLib
SensorSignal/SensorSignal.o: SensorSignal/SensorSignal.cpp
	$(MAKE) -C SensorSignal
Server/ServerSocket.o: Server/ServerSocket.cpp
	$(MAKE) -C Server
Client/ClientSocket.o: Client/ClientSocket.cpp
	$(MAKE) -C Client
Socket/Socket.o: Socket/Socket.cpp
	$(MAKE) -C Socket

clean:
	rm -rf *.o analog_stream test_main
	rm -rf bin/test_main bin/analog_stream
	$(MAKE) -C BlackLib clean
	$(MAKE) -C cron clean
	$(MAKE) -C SensorSignal clean
	$(MAKE) -C Server clean
	$(MAKE) -C Client clean
	$(MAKE) -C Socket clean
