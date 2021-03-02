#include <iostream>
#include <mpi.h>
#include <unistd.h>

int main(int argc, char const *argv[]){
    MPI_Init(NULL, NULL);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank == 0){
        for(int i = 0; i < size/2; i++){
            if((size%2 == 0) && i == (size/2)-1){
                int student;
                MPI_Recv(&student, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(&student, 1, MPI_INT ,student ,0 ,MPI_COMM_WORLD);
            }
            else{
                int students[2];
                for(int j = 0; j < 2; j++)
                    MPI_Recv(&students[j], 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                for(int j = 0; j < 2 ; j++){
                    MPI_Send(&students[1-j], 1, MPI_INT ,students[j] ,0 ,MPI_COMM_WORLD);
                }
            }
        }
        for(int i = 1; i < size; i++){
            char message[128] = {};
            MPI_Send(&rank ,1 ,MPI_INT ,i ,0 ,MPI_COMM_WORLD);
            MPI_Recv(&message ,128 ,MPI_CHAR ,MPI_ANY_SOURCE ,MPI_ANY_TAG ,MPI_COMM_WORLD ,MPI_STATUS_IGNORE);
            std::cout << message << std::endl;
        }
    }else{
        int partner = 0;
        MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&partner, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::string message;
        if(rank == partner)
            message = "I am student " + std::to_string(rank) + " and I have no partner";
        else
            message = "I am student " + std::to_string(rank) + " and my partner is student " + std::to_string(partner);
        MPI_Recv(&partner, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(message.c_str(), message.length(), MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
