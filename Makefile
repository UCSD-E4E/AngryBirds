GCC=g++
CFLAGS= `pkg-config --cflags opencv`
LDFLAGS= `pkg-config --libs opencv`

all: main

main: main.cpp
	$(GCC) $(CFLAGS) main.cpp $(LDFLAGS) -o main


clean:
	rm -f main
