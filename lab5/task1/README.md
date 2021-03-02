# Distributed Pairing 1 (a client-server application) 
Assume that a class has n students numbered 1 to n and one teacher. The teacher wants to assign the students to groups of two for a project. The teacher does so by having every student submit a request for a partner. The teacher takes the first two requests, forms a group from those students, and lets each student know his/her partner. The teacher then takes the next two requests, forms a second group, and so on. Ifn is odd, the last student "partners" with himself/herself.

Model the students as client processes and the teacher as a server process. Develop and implement a distributed client-server application that includes the server (teacher) and clients (students). When your application terminates each student process should print his/her own index (a number or a name) and the index (a number or a name) of the student's partner. You can implement your distributed application in C using the MPI library or in Java using the Java socket API or Java RMI. Your application must be distributed. In comments in the program or in a README file, shortly explain your solution (algorithm). 

## How to run
Make the project with: 
    make

and then run with:
    mpirun -n x ./clients

where *x* is the number of clients to run.