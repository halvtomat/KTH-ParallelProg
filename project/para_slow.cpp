#include <iostream>
#include <random>
#include "common.h"

#define DEFAULT_NUM_BODIES 100
#define DEFAULT_NUM_STEPS 100
#define G 6.67e-5
#define SOFTENING 1e-2
#define DT 1.0
#define WORLD_SIZE 1000
#define SPREAD 100 // 0 - 100
#define MAX_SPEED 5.0
#define DELAY 10
#define DRAW_SIZE 3

int gnumBodies = 0;
int numSteps = 0;

point_t *p;
point_t *v;
point_t *f;
double *m;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Event event;

bool running = true;
bool draw = false;
bool godPoint = false;

int numWorkers = 4;

void initialize_bodies();
void calculate_forces();
void move_bodies();
void draw_bodies();
void print_help();
void exit();

int main(int argc, char const *argv[]){
    if(read_bool_argument(argc, argv, "-h", false))
        print_help();

    gnumBodies = read_int_argument(argc, argv, "-b", DEFAULT_NUM_BODIES);
    numSteps = read_int_argument(argc, argv, "-n", DEFAULT_NUM_STEPS);
    draw = read_bool_argument(argc, argv, "-d", false);
    godPoint = read_bool_argument(argc, argv, "-g", false);
    numWorkers = read_int_argument(argc, argv, "-w", 4);

    p = (point_t *)malloc(sizeof(point_t)*gnumBodies);
    v = (point_t *)malloc(sizeof(point_t)*gnumBodies);
    f = (point_t *)malloc(sizeof(point_t)*numWorkers*gnumBodies);
    m = (double *)malloc(sizeof(double)*gnumBodies);

    omp_set_num_threads(numWorkers);

    if(draw){
        if(SDL_Init(SDL_INIT_VIDEO) < 0){
            std::cout << SDL_GetError() << "\n";
            return 1;
        }
        window = SDL_CreateWindow("N-body simulation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
        if(window ==  NULL){
            std::cout << SDL_GetError() << "\n";
            return 1;
        }
        renderer = SDL_CreateRenderer(window, -1, 0);
        if(renderer == NULL){
            std::cout << SDL_GetError() << "\n";
            return 1;
        }
    }

    initialize_bodies();

    double startTime = omp_get_wtime();
    for(int i = 0; i < numSteps; i++){
        calculate_forces();
        if(draw){
            draw_bodies();
            SDL_Delay(DELAY);
            if(running == false)
                return 0;
        }
        move_bodies();
    }
    double endTime = omp_get_wtime();

    std::cout << "NUM BODIES = " << gnumBodies << " , NUM STEPS = " << numSteps << " , TIME = " << endTime - startTime << std::endl;
    exit(draw);
}

void initialize_bodies(){
    std::random_device dev;
    std::mt19937 rng(dev());
    std::default_random_engine re;
    std::uniform_int_distribution<std::mt19937::result_type> distP(WORLD_SIZE/2 - WORLD_SIZE/2 * SPREAD/100, WORLD_SIZE/2 + WORLD_SIZE/2 * SPREAD/100);
    std::uniform_real_distribution<double> distV(-1.0 * MAX_SPEED, MAX_SPEED);

    for(int i = 0; i < gnumBodies; i++){
        p[i].x = distP(re);
        p[i].y = distP(re);
        v[i].x = distV(re);
        v[i].y = distV(re);
        f[i].x = 0.0;
        f[i].y = 0.0;
        m[i] = 1.0;
    }
    if(godPoint){
        m[0] = 100000000.0;
        p[0].x = WORLD_SIZE/2;
        p[0].y = WORLD_SIZE/2;
        v[0].x = 0.0;
        v[0].y = 0.0;
    }
}

void calculate_forces(){
    #pragma omp parallel
    {
        double distance, magnitude; 
        point_t direction;
        #pragma omp for
        for(int i = 0; i < gnumBodies-1; i++){
            for(int j = i+1; j < gnumBodies; j++){
                distance = point_distance(p[i], p[j]);
                magnitude = calc_magnitude(m[i], m[j], distance, G, SOFTENING);
                direction = point_direction(p[i], p[j]);
                int offset = gnumBodies*omp_get_thread_num();
                (f + offset + i)->x += magnitude*direction.x/distance;
                (f + offset + j)->x -= magnitude*direction.x/distance;
                (f + offset + i)->y += magnitude*direction.y/distance;
                (f + offset + j)->y -= magnitude*direction.y/distance;
            }
        }
    }

}

void move_bodies(){
    #pragma omp parallel
    {
        point_t deltav;
        point_t deltap;
        point_t force;
        #pragma omp for 
        for(int i = 0; i < gnumBodies; i++){
            for(int j = 0; j < numWorkers; j++){
                force.x += (f + gnumBodies*j + i)->x;
                force.y += (f + gnumBodies*j + i)->y;
                (f + gnumBodies*j + i)->x = 0.0;
                (f + gnumBodies*j + i)->y = 0.0;
            }
            deltav = point_deltav(force, m[i], DT);
            deltap = point_deltap(v[i], deltav, DT);
            v[i].x += deltav.x;
            v[i].y += deltav.y;
            p[i].x += deltap.x;
            p[i].y += deltap.y;
            force.x = 0.0;
            force.y = 0.0;
        }
    }

}

void draw_bodies(){
    while(SDL_PollEvent(&event) != 0){
        if(event.type == SDL_QUIT)
            exit(true);
    }
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);
    
    if(godPoint){
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x30, 0x30, 0xFF);
        SDL_Rect rect = {(int)p[0].x, (int)p[0].y, DRAW_SIZE*2, DRAW_SIZE*2};
        SDL_RenderFillRect(renderer, &rect);
    }
    
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    for(int i = 0 + godPoint; i < gnumBodies; i++){
        SDL_Rect rect = {(int)p[i].x, (int)p[i].y, DRAW_SIZE, DRAW_SIZE};
        SDL_RenderFillRect(renderer, &rect);
    }
    SDL_RenderPresent(renderer);
}

void print_help(){
    std::cout << "Sequential N-bodies simulation\n\n";
    std::cout << "-h        : displays this helper text\n";
    std::cout << "-b <x>    : set number of bodies to x. default = " << DEFAULT_NUM_BODIES << "\n";
    std::cout << "-n <x>    : set number of steps to x. default = " << DEFAULT_NUM_STEPS << "\n";
    std::cout << "-d        : draw the point in each iteration. default = false\n";
    std::cout << "-g        : make point[0] a god point with 100000000x more mass than the others. default = false\n";
    std::cout << "-w <x>    : set number of workers (threads) to simultainiusly run the simulation. default = 4\n";
    std::cout << "\n";
}

void exit(bool draw){
    free(p);
    free(v);
    free(f);
    free(m);
    if(draw)
        exit_SDL();
    running = false;
}