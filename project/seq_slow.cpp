#include <iostream>
#include "common.h"

int gnumBodies = 0;

Bodies *bodies;
point_t *f;

void calculate_forces();
void move_bodies();
void exit();

int main(int argc, char const *argv[]){
    if(read_bool_argument(argc, argv, "-h", false)){
        std::cout << help_message();
        return 0;
    }
        

    gnumBodies = read_int_argument(argc, argv, "-b", DEFAULT_NUM_BODIES);
    int numSteps = read_int_argument(argc, argv, "-n", DEFAULT_NUM_STEPS);
    bool draw = read_bool_argument(argc, argv, "-d", false);
    bool godPoint = read_bool_argument(argc, argv, "-g", false);

    f = (point_t *)malloc(sizeof(point_t)*gnumBodies);

    bodies = new Bodies(
        gnumBodies,
        MAX_SPEED,
        MAX_MASS,
        SPREAD,
        godPoint,
        draw,
        DELAY,
        DRAW_SIZE,
        WINDOW_HEIGHT,
        WINDOW_WIDTH);

    double startTime = omp_get_wtime();
    for(int i = 0; i < numSteps; i++){
        calculate_forces();
        if(draw){
            bodies->draw_bodies();
            if(bodies->running == false){
                numSteps = i;
                break;
            }
        }
        move_bodies();
    }
    double endTime = omp_get_wtime();

    std::cout << "NUM BODIES = " << gnumBodies << " , NUM STEPS = " << numSteps << " , TIME = " << endTime - startTime << std::endl;
    exit();
    return 0;
}

void calculate_forces(){
    double distance, magnitude; 
    point_t direction;
    for(int i = 0; i < gnumBodies-1; i++){
        for(int j = i+1; j < gnumBodies; j++){
            distance = point_distance(bodies->p[i], bodies->p[j]);
            magnitude = calc_magnitude(bodies->m[i], bodies->m[j], distance, G, SOFTENING);
            direction = point_direction(bodies->p[i], bodies->p[j]);
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
        deltav = point_deltav(f[i], bodies->m[i], DT);
        deltap = point_deltap(bodies->v[i], deltav, DT);
        bodies->v[i].x += deltav.x;
        bodies->v[i].y += deltav.y;
        bodies->p[i].x += deltap.x;
        bodies->p[i].y += deltap.y;
        f[i].x = 0.0;
        f[i].y = 0.0;
    }
}

void exit(){
    free(f);
    bodies->~Bodies();
}