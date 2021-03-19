#include <iostream>
#include "quad_tree.h"

#define ARENA_SIZE SPREAD * 2
#define ARENA_START -SPREAD

int gnumBodies = 0;
double far = 0;

Bodies *bodies;
point_t *f;

QuadTree* build_tree();
void calculate_forces();
point_t force_util(node_t* node, point_t p, double mass, int id);
void move_bodies();
void exit();

int main(int argc, char const *argv[]){
    if(read_bool_argument(argc, argv, "-h", false)){
        std::cout << help_message();
        return 0;
    }
        
    gnumBodies = read_int_argument(argc, argv, "-b", DEFAULT_NUM_BODIES);
    far = read_double_argument(argc, argv, "-f", DEFAULT_FAR);
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

void print_tree(node_t* node, int x){
    if(node){
        for(int i = 0; i < x; i++)
            std::cout << "\t";
        std::cout << "id=" << node->id << " center=(" << node->center.x << "," << node->center.y << ")\n";
        print_tree(node->upLeft, x+1);
        print_tree(node->downLeft, x+1);
        print_tree(node->upRight, x+1);
        print_tree(node->downRight, x+1);
    }
}

QuadTree* build_tree(){
    point_t start{ARENA_START, ARENA_START};
    QuadTree* tree = new QuadTree(start, ARENA_SIZE);
    for(int i = 0; i < gnumBodies; i++){
        tree->insert(bodies->p[i], bodies->m[i], i);
        print_tree(tree->root, 0);
        std::cout << "\n";
    }
    return tree;
}

void calculate_forces(){
    QuadTree* tree = build_tree();
    //print_tree(tree->root, 0);
    for(int i = 0; i < gnumBodies; i++){
        point_t force = force_util(tree->root, bodies->p[i], bodies->m[i], i);
        f[i].x = force.x;
        f[i].y = force.y;
    }
    tree->~QuadTree();
}

point_t force_util(node_t* node, point_t p, double mass, int id){
    point_t force;
    force.x = 0;
    force.y = 0;
    double distance = point_distance(p, node->center);
    double size = (ARENA_SIZE / pow(2, node->depth));
    double q = size / distance;
    if((node->isPoint || q < far) && node->id != id){
        double magnitude = calc_magnitude(mass, node->mass, distance, G, SOFTENING);
        point_t direction = point_direction(p, node->center);
        force.x = -magnitude*direction.x/distance;
        force.y = -magnitude*direction.y/distance;
    }else{
        if(node->upLeft != nullptr){
            point_t forceNew = force_util(node->upLeft, p, mass, id);
            force.x += forceNew.x;
            force.y += forceNew.y;
        }
        if(node->downLeft != nullptr){
            point_t forceNew = force_util(node->downLeft, p, mass, id);
            force.x += forceNew.x;
            force.y += forceNew.y;
        }
        if(node->upRight != nullptr){
            point_t forceNew = force_util(node->upRight, p, mass, id);
            force.x += forceNew.x;
            force.y += forceNew.y;
        }
        if(node->downRight != nullptr){
            point_t forceNew = force_util(node->downRight, p, mass, id);
            force.x += forceNew.x;
            force.y += forceNew.y;
        }
    }
    return force;
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