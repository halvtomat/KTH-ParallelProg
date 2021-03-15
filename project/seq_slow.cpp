#include <iostream>
#include <random>
#include "common.h"

#define DEFAULT_NUM_BODIES 100
#define DEFAULT_NUM_STEPS 100
#define G 100
#define DT 1.0
#define WORLD_SIZE 1000
#define SPREAD 100 // 0 - 100
#define MAX_SPEED 5.0
#define DELAY 100
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
    bool draw = read_bool_argument(argc, argv, "-d", false);

    p = (point_t *)malloc(sizeof(point_t)*gnumBodies);
    v = (point_t *)malloc(sizeof(point_t)*gnumBodies);
    f = (point_t *)malloc(sizeof(point_t)*gnumBodies);
    m = (double *)malloc(sizeof(double)*gnumBodies);

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

    double startTime = 0;
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
    double endTime = 1;

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
}

void calculate_forces(){
    double distance, magnitude; 
    point_t direction;
    for(int i = 0; i < gnumBodies-1; i++){
        for(int j = i+1; j < gnumBodies; j++){
            distance = point_distance(p[i], p[j]);
            magnitude = calc_magnitude(m[i], m[j], distance, G);
            direction = point_direction(p[i], p[j]);
            f[i].x += magnitude*direction.x/distance;
            f[j].x -= magnitude*direction.x/distance;
            f[i].y += magnitude*direction.y/distance;
            f[j].y -= magnitude*direction.y/distance;
        }
    }
}

void move_bodies(){
    point_t deltav;
    point_t deltap;
    for(int i = 0; i < gnumBodies; i++){
        deltav = point_deltav(f[i], m[i], DT);
        deltap = point_deltap(v[i], deltav, DT);
        v[i].x += deltav.x;
        v[i].y += deltav.y;
        p[i].x += deltap.x;
        p[i].y += deltap.y;
        f[i].x = 0.0;
        f[i].y = 0.0;
    }
}

void draw_bodies(){
    while(SDL_PollEvent(&event) != 0){
        if(event.type == SDL_QUIT)
            exit(true);
    }
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    for(int i = 0; i < gnumBodies; i++){
        SDL_Rect rect = {p[i].x, p[i].y, DRAW_SIZE, DRAW_SIZE};
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