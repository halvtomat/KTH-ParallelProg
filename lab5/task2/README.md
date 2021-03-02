# Distributed Pairing 2 (a peer-to-peer application)
As in the previous problem, we want to assign n students numbered 1 to n to groups of two for a project. However, the teacher does not want to be involved in forming the groups. Instead, he asks the students to interact with each other to pair up into groups. In particular, the students have to come up with a single algorithm that every student executes. Develop a distributed pairing algorithm for the students. An ideal solution will use only n messages. The algorithm should use random numbers so that the outcome is non-deterministic. The teacher starts the algorithm by picking one student at random and sending a message to that student saying in effect "your turn to pick a partner". 

Model the students and the teachers as distributed processes. Develop and implement a distributed peer-to-peer application that includes the teacher peer and a number of student peers executing the distributed pairing algorithm that you have developed. When your application terminates each student process should print his/her own index (a number or a name) and the index (a number or a name) of the student's partner. You can implement your distributed application in C using the MPI library or in Java using the socket API or Java RMI. Your application must be distributed. In comments in the program or in a README file, shortly explain your solution (algorithm). 

## How to run
Make the project with: 
    make

and then run with:
    mpirun -n x ./clients

where *x* is the number of clients to run.