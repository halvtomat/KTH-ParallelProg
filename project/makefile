CC = g++
CFLAGS = -O3 -Wall -fopenmp -lSDL2
TARGETS = seq_slow para_slow

all: $(TARGETS) 

para_slow: para_slow.o common.o
	$(CC) -o $@ para_slow.o common.o $(CFLAGS)

seq_slow: seq_slow.o common.o
	$(CC) -o $@ seq_slow.o common.o $(CFLAGS)



%.o: %.cpp
	$(CC) -c $*.cpp $(CFLAGS)

clean:
	rm -rf *.o $(TARGETS)