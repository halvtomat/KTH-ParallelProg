#include <pthread.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct args_t{
    std::string line1;
    std::string line2;
};
struct return_t{
    std::string line1;
    std::string line2;
    bool equal;
};

void *compare_lines(void *void_args){
    args_t *args = (args_t *)void_args;
    return_t ret;
    ret.line1 = args->line1;
    ret.line2 = args->line2;
    ret.equal = false;
    if(args->line1 == args->line2)
        ret.equal = true;
    return new return_t(ret);
}

int main(int argc, char const *argv[]){
    if(argc != 3){
        std::cout << "wrong number of arguments\n";
        return 1;
    }
        
    std::vector<pthread_t> threads_vector;
    std::ifstream file1, file2;

    file1.open(argv[1]);
    file2.open(argv[2]);

    while(file1.eof() == false && file2.eof() == false){
        args_t args;
        getline(file1, args.line1);
        getline(file2, args.line2);
        pthread_t thread;
        pthread_create(&thread, NULL, compare_lines, new args_t(args));
        threads_vector.push_back(thread);
    }
    for(int i = 0; i < threads_vector.size(); i++){
        return_t *ret;
        pthread_join(threads_vector[i], (void **)&ret);
        if(ret->equal == false){
            std::cout << "< " << ret->line1 << "\n> " << ret->line2 << "\n";
        }
    }
    if(file1.eof() == false){
        std::string line;
        while(getline(file1, line))
            std::cout << "< "<< line << "\n";
    }
    if(file2.eof() == false){
        std::string line;
        while(getline(file2, line))
            std::cout << "> " << line << "\n";
    }
    return 0;
}
