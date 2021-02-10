#include<iostream>
#include<string>
#include<omp.h>

#define N 8

int solution_count = 0;
bool print = false;
bool data = false;
int queen_positions[N] = {0};
int thread_tasks[8];
int solutions[8];

void print_solution(int qp[N]){
    std::cout << "[";
    for(int i = 0; i < N; i++){
        std::cout << qp[i];
        if(i < N-1)
            std::cout << ", ";
    }
    std::cout << "]\n";
}

void check_solution(int qp[N]){
    thread_tasks[omp_get_thread_num()]++;
    int a,b;
    for(int i = 0; i < N; i++){
        a = qp[i];
        for(int j = i + 1; j < N; j++){
            b = qp[j];
            if(a == b || a == b + (j - i) || a == b - (j - i))
                return;
        }
    }
    if(print)
        print_solution(qp);
    solutions[omp_get_thread_num()]++;
}

void find_solutions(int k){
    if(k == N){
        #pragma omp task firstprivate(queen_positions)
        check_solution(queen_positions);
    }
        
    else{
        for(int i = 0; i < N; i++){
            queen_positions[k] = i;
            find_solutions(k+1);
        }
    }
}

int main(int argc, char const *argv[]){
    double start, finish;

    if(argc == 3){
        if(argv[2][0] == '1')
            print = true;
        else if(argv[2][0] == '2')
            data = true;
    }
        
    omp_set_num_threads(atoi(argv[1]));

    start = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single nowait 
        {
            find_solutions(0);
        }
    }
    #pragma omp taskwait
    finish = omp_get_wtime();

    if(data == false){
        int sum = 0;
        for(int i = 0; i < 8; i++){
            sum += thread_tasks[i];
            solution_count += solutions[i];
        std::cout << "thread " << i << " tasks = " << thread_tasks[i] << std::endl; 
        }
        std::cout << "total tasks = " << sum << std::endl;
        std::cout << "solution count = " << solution_count << std::endl;
        std::cout << "execution time = " << finish - start << std::endl;
    }
    else
        std::cout << finish - start << std::endl;

    return 0;
}
