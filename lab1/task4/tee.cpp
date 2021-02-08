#include <iostream>
#include <fstream>
#include <pthread.h>
#include <string.h>

#define BUFFER_SIZE 8

char* buffer;
bool write1;
bool write2;
bool end;
pthread_mutex_t lock;
pthread_cond_t producer_cond; 
pthread_cond_t consumer1_cond;
pthread_cond_t consumer2_cond;


void init(){
    buffer = (char *)malloc(BUFFER_SIZE);
    write1 = true;
    write2 = true;
    end = false;
    lock = PTHREAD_MUTEX_INITIALIZER;
    producer_cond = PTHREAD_COND_INITIALIZER;
    consumer1_cond = PTHREAD_COND_INITIALIZER;
    consumer2_cond = PTHREAD_COND_INITIALIZER;
}

void* read(void* arg){
    while(std::cin.eof() == false){
        pthread_mutex_lock(&lock);
        while(write1 == false || write2 == false)
            pthread_cond_wait(&producer_cond, &lock);
        write1 = false;
        write2 = false;
        memset(buffer, 0, BUFFER_SIZE);
        std::cin.read(buffer, BUFFER_SIZE);
        pthread_cond_signal(&consumer1_cond);
        pthread_cond_signal(&consumer2_cond);
        pthread_mutex_unlock(&lock);
    }
    end = true;
    return NULL;
}

void* write_stdout(void* arg){
    while(end == false || write1 == false){
        pthread_mutex_lock(&lock);
        while(write1 == true)
            pthread_cond_wait(&consumer1_cond, &lock);  
        write1 = true;
        std::cout << buffer;
        pthread_cond_signal(&producer_cond);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void* write_file(void* arg){
    std::ofstream file;
    file.open((char *)arg);
    while(end == false || write2 == false){
        pthread_mutex_lock(&lock);
        while(write2 == true)
            pthread_cond_wait(&consumer2_cond, &lock);
        write2 = true;
        file << buffer;
        pthread_cond_signal(&producer_cond);
        pthread_mutex_unlock(&lock);
    }
    file.flush();
    file.close();
    return NULL;
}

int main(int argc, char const *argv[]){

    init();

    pthread_t t_read;
    pthread_t t_write_stdout;
    pthread_t t_write_file;

    pthread_create(&t_read, NULL, read, NULL);
    pthread_create(&t_write_stdout, NULL, write_stdout, NULL);
    pthread_create(&t_write_file, NULL, write_file, (void *)argv[1]);

    pthread_join(t_read, NULL);
    pthread_join(t_write_stdout, NULL);
    pthread_join(t_write_file, NULL);

    free(buffer);
    return 0;
}