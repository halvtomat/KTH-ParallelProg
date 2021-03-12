#include "common.h"

double point_distance(point_t a, point_t b){
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

double calc_magnitude(double a, double b, double distance, double G){
    return (G*a*b) / pow(distance, 2);
}

point_t point_direction(point_t a, point_t b){
    point_t direction;
    direction.x = b.x - a.x;
    direction.y = b.y - a.y;
    return direction;
}

point_t point_deltav(point_t f, double m, double DT){
    point_t deltav;
    deltav.x = f.x/m * DT;
    deltav.x = f.y/m * DT;
    return deltav;
}

point_t point_deltap(point_t v, point_t deltav, double DT){
    point_t deltap;
    deltap.x = (v.x + deltav.x/2) * DT;
    deltap.x = (v.y + deltav.y/2) * DT;
    return deltap;
}

int read_int_argument(int argc, const char **argv, std::string identifier, int defaul_value){
    int arg = defaul_value;
    for(int i = 1; i < argc; i++)
        if(identifier == argv[i])
            arg = atoi(argv[i+1]);
    return arg;
}

bool read_bool_argument(int argc, const char **argv, std::string identifier, bool default_value){
    bool arg = default_value;
    for(int i = 1; i < argc; i++)
        if(identifier == argv[i])
            arg = true;
    return arg;
}

std::string read_string_argument(int argc, const char **argv, std::string identifier, std::string default_value){
    std::string arg = default_value;
    for(int i = 1; i < argc; i++)
        if(identifier == argv[i])
            arg = argv[i+1];
    return arg;
}