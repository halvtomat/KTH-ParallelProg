#include <iostream>
#include <mpi.h>
#include <random>

std::random_device dev;
std::mt19937 rng(dev());

int random_student(int size, int *students, int caller){
    std::uniform_int_distribution<std::mt19937::result_type> dist(1, students[0]);
    int rand = dist(rng);
    while(students[rand] != 0 || rand == caller){
        rand++;
        if(rand == size)
            rand = 1;
    }
    return rand;
}

int main(int argc, char const *argv[]){
    MPI_Init(NULL, NULL);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::uniform_int_distribution<std::mt19937::result_type> dist(1, size-1);

    if(rank == 0){ //Teacher
        int students[size]; //students[0] contains the amount of students without a partner
        students[0] = size-1;
        for(int i = 1; i < size; i++)
            students[i] = 0;
        MPI_Send(&students ,size ,MPI_INT ,dist(rng) ,0 ,MPI_COMM_WORLD);
        for(int i = 1; i < size; i++){
            char message[128] = {};
            MPI_Recv(&message ,128 ,MPI_CHAR ,MPI_ANY_SOURCE ,MPI_ANY_TAG ,MPI_COMM_WORLD ,MPI_STATUS_IGNORE);
            std::cout << message << std::endl;
        }
    }else{ //Student
        int partner = 0;
        int students[size];
        MPI_Recv(&students ,size ,MPI_INT ,MPI_ANY_SOURCE ,0 ,MPI_COMM_WORLD ,MPI_STATUS_IGNORE);
        if(students[rank] == 0){
            if(students[0] != 1){
                partner = random_student(size, students, rank);
                students[rank] = partner;
                students[partner] = rank;
                students[0] -= 2;
                MPI_Send(&students ,size ,MPI_INT ,partner ,0 ,MPI_COMM_WORLD);
            }else
                partner = rank;            
        }else{
            partner = students[rank];
            if(students[0] != 0){
                int next_student = random_student(size, students, rank);
                MPI_Send(&students ,size ,MPI_INT ,next_student ,0 ,MPI_COMM_WORLD);
            }
        }
        std::string message;
        if(rank == partner)
            message = "I am student " + std::to_string(rank) + " and I have no partner";
        else
            message = "I am student " + std::to_string(rank) + " and my partner is student " + std::to_string(partner);
        MPI_Send(message.c_str(), message.length(), MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}

