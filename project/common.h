#ifndef COMMON_H
#define COMMON_H

#include <cstdlib>
#include <string>
#include <cmath>
#include <random>
#include <SDL2/SDL.h>
#include <omp.h>
#include <iostream>
#include "point.h"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define DEFAULT_NUM_BODIES 100
#define DEFAULT_NUM_STEPS 100
#define DEFAULT_NUM_WORKERS 4
#define DEFAULT_FAR 0.5
#define G 6.67e-10
#define SOFTENING 1e0
#define DT 1.0
#define SPREAD 500 
#define MAX_SPEED 1e0
#define MAX_MASS 1e5
#define DELAY 0
#define DRAW_SIZE 2
#define ZOOM_FACTOR 0.1

class Bodies{
    private:
        int gnumBodies;
        bool godPoint;
        double maxSpeed;
        double maxMass;
        double spread;

        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Event event;
        int frameDelay;
        int drawSize;
        int windowHeight;
        int windowWidth;
        bool draw;

        void initialize_bodies();
    public:
        point_t *p;
        point_t *v;
        double *m;
        bool running;

        Bodies( int gnumBodies,
                double maxSpeed,
                double maxMass,
                double spread,
                bool godPoint=false, 
                bool draw=false, 
                int frameDelay=0, 
                int drawSize=0, 
                int windowHeight=0, 
                int windowWidth=0);
        ~Bodies();
        void draw_bodies();
};

double point_distance(point_t a, point_t b);
double calc_magnitude(double a, double b, double distance, double gravity, double softening);
point_t point_direction(point_t a, point_t b);
point_t point_deltav(point_t f, double m, double deltaTime);
point_t point_deltap(point_t v, point_t deltav, double deltaTime);

double read_double_argument(int argc, const char **argv, std::string identifier, double default_value);
int read_int_argument(int argc, const char **argv, std::string identifier, int defaul_value);
bool read_bool_argument(int argc, const char **argv, std::string identifier, bool default_value);
std::string read_string_argument(int argc, const char **argv, std::string identifier, std::string default_value);

std::string help_message();

#endif