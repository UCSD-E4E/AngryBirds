GCC=g++
CFLAGS= `pkg-config --cflags opencv`
LDFLAGS= `pkg-config --libs opencv`

all: main

main: main.o
	$(GCC) $(CFLAGS) main.o $(LDFLAGS)

main.o: main.cpp
	$(GCC) $(CFLAGS) main.cpp 


clean:
	rm -f main main.o
