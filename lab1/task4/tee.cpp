#include <iostream>
#include <fstream>
#include <pthread.h>
#include <string>

int BUFFER_SIZE = 8;
char* buffer;
pthread_mutex_t buffer_lock;
pthread_cond_t buffer_cond;
bool write1, write2, end;

void init(){
    pthread_mutex_init(&buffer_lock, NULL);
    pthread_cond_init(&buffer_cond, NULL);
    write1 = false;
    write2 = false;
    end = false;
    buffer = (char *)malloc(BUFFER_SIZE);
}

void* read(void* arg){
    while(std::cin.eof() == false && std::cin.fail() == false){
        pthread_mutex_lock(&buffer_lock);
        while(write1 == false && write2 == false)
            pthread_cond_wait(&buffer_cond, &buffer_lock);
        write1 = false;
        write2 = false;
        std::cin.get(buffer, BUFFER_SIZE);
        pthread_cond_signal(&buffer_cond);
        pthread_mutex_unlock(&buffer_lock);
    }
    end = true;
    return NULL;
}

void* write_stdout(void* arg){
    while(end == false){
        pthread_mutex_lock(&buffer_lock);
        while(write1 == true)
            pthread_cond_wait(&buffer_cond, &buffer_lock);
        write1 = true;
        std::cout << buffer;
        pthread_cond_signal(&buffer_cond);
        pthread_mutex_unlock(&buffer_lock);
    }
    return NULL;
}

void* write_file(void* arg){
    std::ofstream file;
    file.open((char *)arg);
    while(end == false){
        pthread_mutex_lock(&buffer_lock);
        while(write2 == true)
            pthread_cond_wait(&buffer_cond, &buffer_lock);
        write2 = true;
        file << buffer;
        pthread_cond_signal(&buffer_cond);
        pthread_mutex_unlock(&buffer_lock);
    }
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