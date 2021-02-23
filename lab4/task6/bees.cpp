#include <iostream>
#include "monitor.h"

monitor mon;

void *consumer(void * args){
    while(true)
        mon.consume();
}

void *producer(void * args){
    int num = *(int *)args;
    mon.start();
    while(true)
        mon.produce(num);
}

int main(int argc, char const *argv[]){
    if(argc != 3){
        std::cout << "Wrong number of arguments, 2 required\n";
        return 1;
    }

    int W = atoi(argv[1]);
    int N = atoi(argv[2]);

    mon = monitor(N, W);
        
    pthread_t bear, bee[N];
    pthread_create(&bear, NULL, consumer, NULL);
    for(int i = 0; i < N; i++){
        int *num = (int *)malloc(sizeof(int));
        *num = i;
        pthread_create(&bee[i], NULL, producer, (void *)num);
    }

    pthread_join(bear, NULL);
    for(int i = 0; i < N; i++)
        pthread_join(bee[i], NULL);

    mon.~monitor();
    return 0;
}
