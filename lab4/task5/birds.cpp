#include <iostream>
#include "monitor.h"

monitor mon;

void *consumer(void * args){
    int num = *(int *)args;
    mon.start();
    while(true)
        mon.consume(num);
}

void *producer(void * args){
    while(true)
        mon.produce();
}

int main(int argc, char const *argv[]){
    if(argc != 3){
        std::cout << "Wrong number of arguments, 2 required\n";
        return 1;
    }

    int W = atoi(argv[1]);
    int N = atoi(argv[2]);

    mon = monitor(N, W);
        
    pthread_t moma, bebe[N];
    pthread_create(&moma, NULL, producer, NULL);
    for(int i = 0; i < N; i++){
        int *num = (int *)malloc(sizeof(int));
        *num = i;
        pthread_create(&bebe[i], NULL, consumer, (void *)num);
    }

    pthread_join(moma, NULL);
    for(int i = 0; i < N; i++)
        pthread_join(bebe[i], NULL);

    mon.~monitor();
    return 0;
}
