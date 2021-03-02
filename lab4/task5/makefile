all: birds.o monitor.o
	g++ birds.o monitor.o -o birds -Wall -pthread

monitor.o: monitor.cpp
	g++ monitor.cpp -c

birds.o: birds.cpp
	g++ birds.cpp -c

clean:
	rm -rf birds *.o

