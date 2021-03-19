CC = g++
CFLAGS = -O3 -Wall -fopenmp -lSDL2
TARGETS = seq_slow para_slow seq_fast

all: $(TARGETS) 

seq_fast: seq_fast.o common.o quad_tree.o
	$(CC) -o $@ seq_fast.o common.o quad_tree.o $(CFLAGS)

para_slow: para_slow.o common.o
	$(CC) -o $@ para_slow.o common.o $(CFLAGS)

seq_slow: seq_slow.o common.o
	$(CC) -o $@ seq_slow.o common.o $(CFLAGS)

%.o: %.cpp
	$(CC) -c $*.cpp $(CFLAGS)

clean:
	rm -rf *.o $(TARGETS)