#include<iostream>
#include<string>
#include<algorithm>
#include<iterator>
#include<vector>
#include<omp.h>

#define N 8
#define MAX_THREADS 8

bool print = false;
bool data = false;
int thread_tasks[MAX_THREADS];
int solutions[MAX_THREADS];

void print_solution(std::vector<int> qp){
    std::cout << "[";
    for(int i = 0; i < N; i++){
        std::cout << qp[i];
        if(i < N-1)
            std::cout << ", ";
    }
    std::cout << "]\n";
}

bool check_solution(std::vector<int> qp){
    thread_tasks[omp_get_thread_num()]++;
    int a,b;
    for(int i = 0; i < N; i++){
        a = qp[i];
        for(int j = i + 1; j < N; j++){
            b = qp[j];
            if(a == b || a == b + (j - i) || a == b - (j - i))
                return false;
        }
    }
    return true;
}

void find_solutions(int k, std::vector<int> qp){
    std::vector<int> qp_copy (N);
    qp_copy = qp;
    if(k == N){
        #pragma omp task firstprivate(qp_copy)
        {
            if(check_solution(qp_copy)){
                if(print){
                    #pragma omp critical
                    print_solution(qp_copy);
                }
                solutions[omp_get_thread_num()]++;
            }
        }
    }
        
    else{
        for(int i = 0; i < N; i++){
            qp[k] = i;
            find_solutions(k+1, qp);
        }
    }
}

int main(int argc, char const *argv[]){
    double start, finish;
    std::vector<int> qp (N);
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
            find_solutions(0, qp);
        }
    }
    #pragma omp taskwait
    finish = omp_get_wtime();

    if(data == false){
        int solution_count = 0;
        int sum = 0;
        for(int i = 0; i < MAX_THREADS; i++){
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
