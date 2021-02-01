# Task 5 The Linux diff command
Develop a parallel multithreaded program (in C using Pthreads, or in Java) that implements a simplified version of the Linux diff command for comparing two text files. The command is invoked as follows:

    diff filename1 filename2

The diff command compares corresponding lines in the given files and prints both lines to the standard output, if the lines are different. If one of the files is longer then the other, diff prints all extra lines in the longer file to the standard output.

## Implementation
My implementation reads both files, line by line, creates a thread for every line, the thread compares the lines and returns if they are the same. Then the main thread prints the differing lines and the potential extra lines to stdout (extra lines = lines in the longer file if one file is longer).

run with the following:

    make
    ./diff filename1 filename2
