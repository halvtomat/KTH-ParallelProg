#include <iostream>
#include <omp.h>

int main(int argc, char const *argv[]){
    int n_threads = atoi(argv[1]);
    omp_set_num_threads(n_threads);
#pragma omp parallel
    {
        int id = omp_get_thread_num();
        std::printf("Hello from thread %d\n", id);
    }
    return 0;
}
