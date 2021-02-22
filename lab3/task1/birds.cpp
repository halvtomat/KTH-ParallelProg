#include <iostream>
#include <vector>
#include <random>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t lock;
sem_t food;
sem_t empty;
sem_t eating;

int N = 0;
int W = 0;
int DISH = 0;

std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<std::mt19937::result_type> dist(1,4);

void *consumer(void * args){
    while(true){
        sem_wait(&eating);
        sem_wait(&food);
        sem_wait(&lock);
        int num = *(int *) args;
        if(DISH == 0){
            sem_post(&empty);
            sem_post(&lock);
            std::cout << "Bebe " << num << " waiting for food...\n";
            sem_wait(&food);
            sem_wait(&lock);
        }
        DISH--;
        std::cout << "Bebe " << num <<  " has eaten\n";
        sem_post(&lock);
        sem_post(&eating);
        sleep(dist(rng));
    }
}

void *producer(void * args){
    while(true){
        sem_wait(&empty);
        sem_wait(&lock);
        for(int i = 0; i < W + 1; i++)
            sem_post(&food);
        DISH = W;
        std::cout << "Moma put food\n";
        sem_post(&lock);
        sleep(dist(rng));
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
    sem_init(&food, 0, W + 1);
    sem_init(&empty, 0, 0);
    sem_init(&eating, 0, 1);

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
