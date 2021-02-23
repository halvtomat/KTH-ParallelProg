all: bees.o monitor.o
	g++ bees.o monitor.o -o bees -Wall -pthread

monitor.o: monitor.cpp
	g++ monitor.cpp -c

bees.o: bees.cpp
	g++ bees.cpp -c

optimized:
	g++ bees.cpp -o bees -Wall -pthread -O3

clean:
	rm -rf bees *.o

