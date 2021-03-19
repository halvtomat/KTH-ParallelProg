#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include "common.h"

#define SINGULARITY_THRESHOLD 1e-8

struct node_t{
    double mass;
    point_t center;
    point_t start;
    int depth;
    bool isPoint;
    int id;
    node_t* upLeft;
    node_t* downLeft;
    node_t* upRight;
    node_t* downRight;
};

class QuadTree{
    private:
        point_t start;

        bool in_bounds(point_t point);
        double center_of_mass(double x1, double x2, double m1, double m2);
        int node_position(point_t start, point_t point, int depth);
        void remove_node(node_t* node);
        node_t* create_node(point_t point, double mass, int depth, point_t start, int id);
        node_t* insert_helper(point_t point, double mass, int id, node_t* current);
    public:
        node_t* root;
        double size;

        QuadTree(point_t start, double size);
        ~QuadTree();
        bool insert(point_t point, double mass, int id);
};

#endif