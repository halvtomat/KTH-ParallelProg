#include <iostream>
#include <random>
#include "common.h"

#define DEFAULT_NUM_BODIES 100
#define DEFAULT_NUM_STEPS 100
#define G 6.67e-11
#define DT 1.0
#define WORLD_SIZE 1000

int gnumBodies = 0;
int numSteps = 0;

point_t *p;
point_t *v;
point_t *f;
double *m;

void initialize_bodies();
void calculate_forces();
void move_bodies();
void draw_bodies();
void print_help();

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

    initialize_bodies();

    double startTime = 0;
    for(int i = 0; i < numSteps; i++){
        calculate_forces();
        if(draw)
            draw_bodies();
        move_bodies();
    }
    double endTime = 1;

    free(p);
    free(v);
    free(f);
    free(m);

    std::cout << "NUM BODIES = " << gnumBodies << " , NUM STEPS = " << numSteps << " , TIME = " << endTime - startTime << std::endl;

    return 0;
}

void initialize_bodies(){
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distP(0, WORLD_SIZE);
    std::uniform_int_distribution<std::mt19937::result_type> distV(0, 10);

    for(int i = 0; i < gnumBodies; i++){
        p[i].x = distP(rng);
        p[i].y = distP(rng);
        v[i].x = distV(rng);
        v[i].y = distV(rng);
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
    std::cout << "--------------------\n\n";
    for(int i = 0; i < gnumBodies; i++){
        std::cout << "p[" << i << "] = (" << p[i].x << ", " << p[i].y << ")\t";
        std::cout << "v[" << i << "] = (" << v[i].x << ", " << v[i].y << ")\t";
        std::cout << "f[" << i << "] = (" << f[i].x << ", " << f[i].y << ")\t";
        std::cout << "\n";
    }
    std::cout << "\n";
}

void print_help(){
    std::cout << "Sequential N-bodies simulation\n\n";
    std::cout << "-h        : displays this helper text\n";
    std::cout << "-b <x>    : set number of bodies to x. default = " << DEFAULT_NUM_BODIES << "\n";
    std::cout << "-n <x>    : set number of steps to x. default = " << DEFAULT_NUM_STEPS << "\n";
    std::cout << "-d        : draw the point in each iteration. default = false\n";
    std::cout << "\n";
}