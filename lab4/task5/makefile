all: birds.o monitor.o
	g++ birds.o monitor.o -o birds -Wall -pthread

monitor.o: monitor.cpp
	g++ monitor.cpp -c

birds.o: birds.cpp
	g++ birds.cpp -c

optimized:
	g++ birds.cpp -o birds -Wall -pthread -O3

clean:
	rm -rf birds

