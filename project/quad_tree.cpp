#include "quad_tree.h"

QuadTree::QuadTree(point_t start, double size){
    this->start.x = start.x;
    this->start.y = start.y;
    this->size = size;
    root = new node_t;
    root->mass = 0;
    root->center.x = 0;
    root->center.y = 0;
    root->start.x = start.x;
    root->start.y = start.y;
    root->depth = 0;
    root->id = -1;
    root->isPoint = false;
    root->upLeft = nullptr;
    root->downLeft = nullptr;
    root->upRight = nullptr;
    root->downRight = nullptr;
}

QuadTree::~QuadTree(){
    remove_node(root);
}

bool QuadTree::insert(point_t point, double mass, int id){
    if(in_bounds(point) == false)
        return false;

    node_t* node = insert_helper(point, mass, id, root);

    if(node == nullptr)
        return false;
    else
        return true;
}

bool QuadTree::in_bounds(point_t point){
    return ((start.x < point.x) && (point.x < (start.x + size))) && ((start.y < point.y) && (point.y < (start.y + size)));
}

double QuadTree::center_of_mass(double x1, double m1, double x2, double m2){
    return (x1*m1 + x2*m2)/(m1*m2);
}

int QuadTree::node_position(point_t start, point_t point, int depth){
    bool xIsBigger = (point.x < start.x + size / pow(2, depth));
    bool yIsBigger = (point.y < start.y + size / pow(2, depth));

    if(xIsBigger){
        if(yIsBigger)
            return 2;
        else
            return 1;
    }else{
        if(yIsBigger)
            return 4;
        else
            return 3;       
    }
}

void QuadTree::remove_node(node_t* node){
    if(node){
        remove_node(node->upLeft);
        remove_node(node->downLeft);
        remove_node(node->upRight);
        remove_node(node->downRight);
        delete node;
    }
}

node_t* QuadTree::create_node(point_t point, double mass, int depth, point_t start, int id){
    node_t* node = new node_t;
    node->center.x = point.x;
    node->center.y = point.y;
    node->mass = mass;
    node->start.x = start.x;
    node->start.y = start.y;
    node->depth = depth;
    node->isPoint = true;
    node->id = id;
    node->upLeft = nullptr;
    node->downLeft = nullptr;
    node->upRight = nullptr;
    node->downRight = nullptr;
    return node;
}

node_t* QuadTree::insert_helper(point_t point, double mass, int id, node_t* current){
    if(current->mass == 0){ // a node with mass == 0 is gonna be the root
        current->mass = mass;
        current->center.x = point.x;
        current->center.y = point.y;
        current->isPoint = true;
        current->id = id;

    }else if(current->depth > MAX_DEPTH){
        return nullptr;

    }else if(current->isPoint){
        int n1 = node_position(current->start, point, current->depth + 1);
        int n2 = node_position(current->start, current->center, current->depth + 1);
        switch (n1){
        case 1:{
            int depth = current->depth + 1;
            point_t start;
            start.x = current->start.x;
            start.y = current->start.y + size / pow(2, depth);
            current->upLeft = create_node(point, mass, depth, start, id);
            }
            break;
        case 2:{
            int depth = current->depth + 1;
            point_t start;
            start.x = current->start.x;
            start.y = current->start.y;
            current->downLeft = create_node(point, mass, depth, start, id);
            }
            break;
        case 3:{
            int depth = current->depth + 1;
            point_t start;
            start.x = current->start.x + size / pow(2, depth);
            start.y = current->start.y + size / pow(2, depth);
            current->upRight = create_node(point, mass, depth, start, id);
            }
            break;
        case 4:{
            int depth = current->depth + 1;
            point_t start;
            start.x = current->start.x + size / pow(2, depth);
            start.y = current->start.y;
            current->downRight = create_node(point, mass, depth, start, id);
            }
            break;
        default:
            break;
        }

        switch (n2){
        case 1:
            if(current->upLeft == nullptr){
            int depth = current->depth + 1;
            point_t start;
            start.x = current->start.x;
            start.y = current->start.y + size / pow(2, depth);
            current->upLeft = create_node(current->center, current->mass, depth, start, current->id);
            }else
                insert_helper(current->center, current->mass, current->id, current->upLeft);
            break;
        case 2: 
            if(current->downLeft == nullptr){
            int depth = current->depth + 1;
            point_t start;
            start.x = current->start.x;
            start.y = current->start.y;
            current->downLeft = create_node(current->center, current->mass, depth, start, current->id);
            }else
                insert_helper(current->center, current->mass, current->id, current->downLeft);
            break;
        case 3: 
            if(current->upRight == nullptr){
            int depth = current->depth + 1;
            point_t start;
            start.x = current->start.x + size / pow(2, depth);
            start.y = current->start.y + size / pow(2, depth);
            current->upRight = create_node(current->center, current->mass, depth, start, current->id);
            }else
                insert_helper(current->center, current->mass, current->id, current->upRight);
            break;
        case 4: 
            if(current->downRight == nullptr){
            int depth = current->depth + 1;
            point_t start;
            start.x = current->start.x + size / pow(2, depth);
            start.y = current->start.y;
            current->downRight = create_node(current->center, current->mass, depth, start, current->id); 
            }else
                insert_helper(current->center, current->mass, current->id, current->downRight);
            break;
        default:
            break;
        }
        
        current->isPoint = false;
        current->id = -1;
        current->center.x = center_of_mass(current->center.x, current->mass, point.x, mass);
        current->center.y = center_of_mass(current->center.y, current->mass, point.y, mass);
        current->mass += mass;

    }else{
        current->center.x = center_of_mass(current->center.x, current->mass, point.x, mass);
        current->center.y = center_of_mass(current->center.y, current->mass, point.y, mass);
        current->mass += mass;

        int n1 = node_position(current->start, current->center, current->depth + 1);

        switch(n1){
        case 1:
            if(current->upLeft == nullptr){
                int depth = current->depth + 1;
                point_t start;
                start.x = current->start.x;
                start.y = current->start.y + size / pow(2, depth);
                current->upLeft = create_node(point, mass, depth, start, id);
            }else
                return insert_helper(point, mass, id, current->upLeft);
            break;
        case 2:
            if(current->downLeft == nullptr){
                int depth = current->depth + 1;
                point_t start;
                start.x = current->start.x;
                start.y = current->start.y;
                current->downLeft = create_node(point, mass, depth, start, id);
            }else
                return insert_helper(point, mass, id, current->downLeft);
            break;
        case 3:
            if(current->upRight == nullptr){
                int depth = current->depth + 1;
                point_t start;
                start.x = current->start.x + size / pow(2, depth);
                start.y = current->start.y + size / pow(2, depth);
                current->upRight = create_node(point, mass, depth, start, id);
            }else
                return insert_helper(point, mass, id, current->upRight);
            break;
        case 4:
            if(current->downRight == nullptr){
                int depth = current->depth + 1;
                point_t start;
                start.x = current->start.x + size / pow(2, depth);
                start.y = current->start.y;
                current->downRight = create_node(point, mass, depth, start, id);
            }else
                return insert_helper(point, mass, id, current->downRight);
            break;
        default:
            return nullptr;
        }
    }
    return current;
}