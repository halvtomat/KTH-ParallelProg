#include<iostream>
#include<string>
#include<algorithm>
#include<iterator>
#include<vector>
#include<omp.h>

#define MAX_THREADS 8

bool print = false;
bool data = false;
int thread_tasks[MAX_THREADS];
int solutions[MAX_THREADS];
int n = 8;

void print_solution(std::vector<int> qp){
    std::cout << "[";
    for(int i = 0; i < n; i++){
        std::cout << qp[i];
        if(i < n-1)
            std::cout << ", ";
    }
    std::cout << "]\n";
}

bool check_solution(std::vector<int> qp){
    thread_tasks[omp_get_thread_num()]++;
    int a,b;
    for(int i = 0; i < n; i++){
        a = qp[i];
        for(int j = i + 1; j < n; j++){
            b = qp[j];
            if(a == b || a == b + (j - i) || a == b - (j - i))
                return false;
        }
    }
    return true;
}

void find_solutions(int k, std::vector<int> qp){
    if(k == n){
        if(check_solution(qp)){
            if(print){
                #pragma omp critical
                print_solution(qp);
            }
            solutions[omp_get_thread_num()]++;
        }
    }
    else{
        for(int i = 0; i < n; i++){
            qp[k] = i;
            find_solutions(k+1, qp);
        }
    }
}

void parallel_solve(){
    for(int i = 0; i < n; i++){
        #pragma omp task
        {
            std::vector<int> qp (n);
            qp[0] = i;
            find_solutions(1, qp);
        }
    }
}

double run(){
    double start, finish;

    start = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single nowait 
        {
            parallel_solve();
        }
    }
    #pragma omp taskwait
    finish = omp_get_wtime();

    return finish - start;
}

void print_result(double result){
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
        std::cout << "execution time = " << result << std::endl;
    }
    else
        std::cout << result << std::endl;
}

int main(int argc, char const *argv[]){
    if(argc == 5){
        if(argv[4][0] == '1')
            print = true;
        else if(argv[4][0] == '2')
            data = true;
    }
    omp_set_num_threads(atoi(argv[1]));
    n = atoi(argv[2]);
    int iterations = atoi(argv[3]);
    double result = 0;

    for(int i = 0; i < iterations; i++)
        result += run();
    result = result/iterations;

    print_result(result);

    return 0;
}
