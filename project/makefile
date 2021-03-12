CC = g++
CFLAGS = -O3 -Wall
TARGETS = seq_slow

all: $(TARGETS) 

seq_slow: seq_slow.o common.o
	$(CC) -o $@ seq_slow.o common.o

%.o: %.cpp
	$(CC) -c $*.cpp $(CFLAGS)

clean:
	rm -rf *.o $(TARGETS)