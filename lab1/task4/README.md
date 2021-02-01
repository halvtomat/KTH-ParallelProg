# Task 4. The Linux tee command
Develop a parallel multithreaded program (in C using Pthreads, or in Java) that implements the Linux tee command, which is invoked as follows:

    tee filename
    
The tee command reads the standard  input and writes it to both the standard output and to the file filename.  Your parallel tee program should have three threads: one for reading standard input, one for writing to standard output, and one for writing to file filename. 

## Implementation
My implementation works by creating three threads, one for reading, one for writing to stdout and one for writing to a file. The two consumers get data from the producer through a common buffer. The producer locks a mutex around the buffer, writes BUFFER_SIZE characters to the buffer and signals that the consumers can start to read from it. When both consumers have read all characters in the buffer they both signal to the producer to keep producing.

To run:

    make
    ./tee filename