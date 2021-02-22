#include "monitor.h"

monitor::monitor(int N, int W){
    n = N;
    w = W;
    dish = w;
    pthread_cond_init(&empty, NULL);
    pthread_cond_init(&full, NULL);
    pthread_mutex_init(&lock, NULL);
}

monitor::~monitor(){
    pthread_cond_destroy(&empty);
    pthread_cond_destroy(&full);
    pthread_mutex_destroy(&lock);
}

void monitor::start(){
    pthread_cond_signal(&full);
}

void monitor::consume(int num){
    pthread_mutex_lock(&lock);
    pthread_cond_wait(&full, &lock);
    std::cout << "food = " << dish << "\n";
    if(dish == 0){
        std::cout << "Bebe " << num << " waiting for food...\n";
        pthread_cond_signal(&empty);
        pthread_cond_wait(&full, &lock);
    }
    dish--;
    std::cout << "Bebe " << num << " has eaten\n";
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&full);
    //sleep(1); //TODO: make random sleep
}

void monitor::produce(){
    pthread_mutex_lock(&lock);
    pthread_cond_wait(&empty, &lock);
    dish = w;
    std::cout << "Mama put food\n";
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&full);
    sleep(1); //TODO: Make random sleep
}