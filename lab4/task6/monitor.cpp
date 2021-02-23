#include "monitor.h"

monitor::monitor(){
    n = 4;
    w = 4;
    pot = 0;
    eating = 0;
    rng = std::mt19937(dev());
    dist = std::uniform_int_distribution<std::mt19937::result_type>(1, 4);
    pthread_cond_init(&empty, NULL);
    pthread_cond_init(&full, NULL);
    pthread_mutex_init(&lock, NULL);
}

monitor::monitor(int N, int W){
    n = N;
    w = W;
    pot = 0;
    eating = 0;
    rng = std::mt19937(dev());
    dist = std::uniform_int_distribution<std::mt19937::result_type>(1, 4);
    pthread_cond_init(&empty, NULL);
    pthread_cond_init(&full, NULL);
    pthread_mutex_init(&lock, NULL);
}

monitor::~monitor(){
    pthread_cond_destroy(&empty);
    pthread_cond_destroy(&full);
    pthread_mutex_destroy(&lock);
}

monitor& monitor::operator=(const monitor & mon){
    n = mon.n;
    w = mon.w;
    pot = 0;
    return *this;
}

void monitor::start(){
    pthread_cond_signal(&empty);
}

void monitor::consume(){
    pthread_mutex_lock(&lock);
    while(!eating)
        pthread_cond_wait(&full, &lock);
    pot = 0;
    std::cout << "Bear has eaten\n";
    eating = 0;
    pthread_cond_signal(&empty);
    pthread_mutex_unlock(&lock);
    sleep(dist(rng));
}

void monitor::produce(int num){
    pthread_mutex_lock(&lock);
    while(eating)
        pthread_cond_wait(&empty, &lock);
    pot++;
    std::cout << "Bee " << num << " made honey\n";
    if(pot == w){
        eating = 1;
        pthread_cond_signal(&full);
        pthread_cond_wait(&empty, &lock);
    }
    pthread_cond_signal(&empty);
    pthread_mutex_unlock(&lock);
    sleep(dist(rng));  
}