#ifndef COMMON_H
#define COMMON_H

#include <cstdlib>
#include <string>
#include <cmath>
#include <SDL2/SDL.h>
#include <omp.h>
#include <iostream>

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 1000;

struct point_t{
    double x;
    double y;
};

void initialize_SDL(SDL_Window *window, SDL_Renderer *renderer);
void exit_SDL();

double point_distance(point_t a, point_t b);
double calc_magnitude(double a, double b, double distance, double G, double softening);
point_t point_direction(point_t a, point_t b);
point_t point_deltav(point_t f, double m, double DT);
point_t point_deltap(point_t v, point_t deltav, double DT);

int read_int_argument(int argc, const char **argv, std::string identifier, int defaul_value);
bool read_bool_argument(int argc, const char **argv, std::string identifier, bool default_value);
std::string read_string_argument(int argc, const char **argv, std::string identifier, std::string default_value);

#endif