#include <iostream>
#include <vector>
#include <random>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t lock;
sem_t food;
sem_t empty;

int N = 0;
int W = 0;
int DISH = 0;

std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<std::mt19937::result_type> dist(0,2);

void *consumer(void * args){
    while(true){
        sem_wait(&food);
        sem_wait(&lock);
        DISH--;
        if(DISH == 0)
            sem_post(&empty);
        int num = *(int *) args;
        std::cout << "Bebe" << num <<  " has eaten\n";
        sleep(dist(rng));
        sem_post(&lock);
    }

}

void *producer(void * args){
    while(true){
        sem_wait(&empty);
        sem_wait(&lock);
        for(int i = 0; i < W; i++)
            sem_post(&food);
        DISH = W;
        std::cout << "Moma put food\n";
        sleep(dist(rng));
        sem_post(&lock);
    }
}

int main(int argc, char const *argv[]){
    if(argc != 3){
        std::cout << "Wrong number of arguments, 2 required\n";
        return 1;
    }
        
    W = atoi(argv[1]);
    N = atoi(argv[2]);
    DISH = W;
    sem_init(&lock, 0, 1);
    sem_init(&food, 0, W);
    sem_init(&empty, 0, 0);

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

    return 0;
}
