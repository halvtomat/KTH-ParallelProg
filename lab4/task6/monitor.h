#ifndef MONITOR_H
#define MONITOR_H

#include <iostream>
#include <random>
#include <unistd.h>
#include <pthread.h>

class monitor{
    pthread_cond_t empty, full;
    pthread_mutex_t lock;
    int n;
    int w;
    int pot;
    int eating;
    std::random_device dev;
    std::mt19937 rng;
    std::uniform_int_distribution<std::mt19937::result_type> dist;
    public:
        monitor();
        monitor(int N, int W);
        ~monitor();
        monitor& operator=(const monitor &);
        void start();
        void consume();
        void produce(int num);
};

#endif