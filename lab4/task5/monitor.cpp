#include "monitor.h"

monitor::monitor(){
    n = 4;
    w = 4;
    dish = w;
    getting_food = 0;
    rng = std::mt19937(dev());
    dist = std::uniform_int_distribution<std::mt19937::result_type>(0, 2);
    pthread_cond_init(&empty, NULL);
    pthread_cond_init(&full, NULL);
    pthread_cond_init(&eat, NULL);
    pthread_mutex_init(&lock, NULL);
}

monitor::monitor(int N, int W){
    n = N;
    w = W;
    dish = w;
    getting_food = 0;
    rng = std::mt19937(dev());
    dist = std::uniform_int_distribution<std::mt19937::result_type>(0, 2);
    pthread_cond_init(&empty, NULL);
    pthread_cond_init(&full, NULL);
    pthread_cond_init(&eat, NULL);
    pthread_mutex_init(&lock, NULL);
}

monitor::~monitor(){
    pthread_cond_destroy(&empty);
    pthread_cond_destroy(&full);
    pthread_cond_destroy(&eat);
    pthread_mutex_destroy(&lock);
}

monitor& monitor::operator=(const monitor & mon){
    n = mon.n;
    w = mon.w;
    dish = w;
    return *this;
}

void monitor::start(){
    pthread_cond_signal(&eat);
}

void monitor::consume(int num){
    pthread_mutex_lock(&lock);
    while(getting_food)
        pthread_cond_wait(&eat, &lock);
    if(dish == 0){
        getting_food = 1;
        std::cout << "Bebe " << num << " waiting for food...\n";
        pthread_cond_signal(&empty);
        pthread_cond_wait(&full, &lock);
    }
    dish--;
    getting_food = 0;
    std::cout << "Bebe " << num << " has eaten\n";
    pthread_cond_signal(&eat);
    pthread_mutex_unlock(&lock);
    sleep(dist(rng));
}

void monitor::produce(){
    pthread_mutex_lock(&lock);
    while(!getting_food)
        pthread_cond_wait(&empty, &lock);
    dish = w;
    std::cout << "Mama put food\n";
    pthread_cond_signal(&full);
    pthread_mutex_unlock(&lock);
    sleep(dist(rng));  
}