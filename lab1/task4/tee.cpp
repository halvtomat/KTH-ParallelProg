#include <iostream>
#include <fstream>
#include <pthread.h>
#include <string>

#define BUFFER_SIZE 8
struct buffer_t {
    char* buffer[BUFFER_SIZE];
    size_t len;
    pthread_mutex_t lock;
    pthread_cond_t producer_cond, consumer_cond; 
};

buffer_t *buffer;

void init(){
    buffer = (buffer_t *)malloc(sizeof(buffer_t));
    *buffer = {
        .len = 0,
        .lock = PTHREAD_MUTEX_INITIALIZER,
        .producer_cond = PTHREAD_COND_INITIALIZER,
        .consumer_cond = PTHREAD_COND_INITIALIZER
    };
}

void* read(void* arg){
    while(std::cin.eof() == false && std::cin.fail() == false){
        pthread_mutex_lock(buffer->lock);
        while(write1 == false || write2 == false){
            std::cout << "write1 = " << write1 << " write2 = " << write2  << " buffer = " << buffer << "\n";
            pthread_cond_wait(&producer_cond, &buffer_lock);
        }

        std::cin.get(buffer, BUFFER_SIZE);
        std::cout << "write1 = " << write1 << " write2 = " << write2  << " buffer = " << buffer << "\n";
        write1 = false;
        write2 = false;
        pthread_cond_signal(&consumer_cond);
        pthread_mutex_unlock(&buffer_lock);

    }
    end = true;
    return NULL;
}

void* write_stdout(void* arg){
    while(end == false){
        pthread_mutex_lock(&buffer_lock);
        while(write1 == true)
            pthread_cond_wait(&consumer_cond, &buffer_lock);
        //std::cout << buffer;
        write1 = true;
        pthread_cond_signal(&producer_cond);
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
            pthread_cond_wait(&consumer_cond, &buffer_lock);
        file.write(buffer,BUFFER_SIZE);
        write2 = true;
        pthread_cond_signal(&producer_cond);
        pthread_mutex_unlock(&buffer_lock);
    }
    file.close();
    return NULL;
}

int main(int argc, char const *argv[]){
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