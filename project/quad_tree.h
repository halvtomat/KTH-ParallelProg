#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include <cmath>

#define SINGULARITY_THRESHOLD 1e-8

struct node_t{
    double mass;
    double centerX;
    double centerY;
    double startX;
    double startY;
    int depth;
    bool isPoint;
    node_t* upLeft;
    node_t* downLeft;
    node_t* upRight;
    node_t* downRight;
};

class QuadTree{
    private:
        node_t* root;
        double size;
        double startX;
        double startY;

        bool in_bounds(double x, double y);
        double center_of_mass(double x1, double x2, double m1, double m2);
        int node_position(double startX, double startY, double x, double y, int depth);
        void remove_node(node_t* node);
        node_t* create_node(double x, double y, double mass, int depth, double startX, double startY);
        node_t* insert_helper(double x, double y, double mass, node_t* current);
    public:
        QuadTree(double startX, double startY, double size);
        ~QuadTree();
        bool insert(double x, double y, double mass);
};

#endif