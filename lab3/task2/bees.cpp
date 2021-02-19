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
int POT = 0;

std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<std::mt19937::result_type> dist(1,4);

void *consumer(void * args){
    while(true){
        sem_wait(&food);
        sem_wait(&lock);
	for(int i = 0; i < W; i++){
            POT--;
            sem_post(&empty);
	}
        std::cout << "Bear has eaten\n";
        sem_post(&lock);
        sleep(dist(rng));
    }

}

void *producer(void * args){
    while(true){
        sem_wait(&empty);
        sem_wait(&lock);
	POT++;
	if(POT == W)
	    sem_post(&food);
	int num = *(int *)args;
        std::cout << "Bee " << num << " put food\n";
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
    POT = W;
    sem_init(&lock, 0, 1);
    sem_init(&food, 0, 1);
    sem_init(&empty, 0, 0);

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

    return 0;
}
