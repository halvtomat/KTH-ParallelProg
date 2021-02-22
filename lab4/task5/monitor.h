#ifndef MONITOR_H
#define MONITOR_H

#include <iostream>
#include <vector>
#include <random>
#include <unistd.h>
#include <pthread.h>

class monitor{
    pthread_cond_t empty, full;
    pthread_mutex_t lock;
    int n;
    int w;
    int dish;
    public:
        monitor(int N, int W);
        ~monitor();
        void start();
        void consume(int num);
        void produce();
};

#endif