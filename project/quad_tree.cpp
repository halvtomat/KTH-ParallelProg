#include "quad_tree.h"

QuadTree::QuadTree(double startX, double startY, double size){
    this->startX = startX;
    this->startY = startY;
    this->size = size;
    root = new node_t;
    root->mass = 0;
    root->centerX = 0;
    root->centerY = 0;
    root->startX = 0;
    root->startY = 0;
    root->depth = 0;
    root->upLeft = nullptr;
    root->downLeft = nullptr;
    root->upRight = nullptr;
    root->downRight = nullptr;
}

QuadTree::~QuadTree(){
    remove_node(root);
}

bool QuadTree::insert(double x, double y, double mass){
    if(in_bounds(x, y) == false)
        return false;

    node_t* node = insert_helper(x, y, mass, root);

    if(node == nullptr)
        return false;
    else
        return true;
}

bool QuadTree::in_bounds(double x, double y){
    return ((startX < x) && (x < (startX + size))) && ((startY < y) && (y < (startY + size)));
}

double QuadTree::center_of_mass(double x1, double x2, double m1, double m2){
    return (x1*m1 + x2*m2)/(m1*m2);
}

int QuadTree::node_position(double startX, double startY, double x, double y, int depth){
    if(x < startX + size / pow(2, depth)){
        if(y < startY + size / pow(2, depth))
            return 2;
        else
            return 1;
    }
    else{
        if(y < startY + size / pow(2, depth))
            return 4;
        else
            return 3;       
    }
}

void QuadTree::remove_node(node_t* node){
    if(node == nullptr)
        return;
    remove_node(node->upLeft);
    remove_node(node->downLeft);
    remove_node(node->upRight);
    remove_node(node->downRight);
    free(node);
}

node_t* QuadTree::insert_helper(double x, double y, double mass, node_t* current){
    if(current->mass == 0){
        current->mass = mass;
        current->centerX = x;
        current->centerY = y;
        current->isPoint = true;
        return current;
    }

    if(current->isPoint){
        node_t* n1 = new node_t;
        node_t* n2 = new node_t;

        n1->depth = current->depth + 1;
        n1->centerX = x;
        n1->centerY = y;
        n1->mass = mass;
        n1->isPoint = true;

        n2->depth = current->depth + 1;
        n2->centerX = current->centerX;
        n2->centerY = current->centerY;
        n2->mass = current->mass;
        n2->isPoint = true;

        switch (node_position(current->startX, current->startY, x, y, current->depth + 1)){
        case 1:
            current->upLeft = n1;
            n1->startX = current->startX;
            n1->startY = current->startY + size / pow(2, n1->depth);
            break;
        case 2:
            current->downLeft = n1;
            n1->startX = current->startX;
            n1->startY = current->startY;
            break;
        case 3:
            current->upRight = n1;
            n1->startX = current->startX + size / pow(2, n1->depth);
            n1->startY = current->startY + size / pow(2, n1->depth);
            break;
        case 4:
            current->downRight = n1;
            n1->startX = current->startX + size / pow(2, n1->depth);
            n1->startY = current->startY;
            break;
        default:
            break;
        }

        switch (node_position(current->startX, current->startY, current->centerX, current->centerY, current->depth + 1)){
        case 1:
            current->upLeft = n2;
            n2->startX = current->startX;
            n2->startY = current->startY + size / pow(2, n2->depth);
            break;
        case 2:
            current->downLeft = n2;
            n2->startX = current->startX;
            n2->startY = current->startY;
            break;
        case 3:
            current->upRight = n2;
            n2->startX = current->startX + size / pow(2, n2->depth);
            n2->startY = current->startY + size / pow(2, n2->depth);
            break;
        case 4:
            current->downRight = n2;
            n2->startX = current->startX + size / pow(2, n2->depth);
            n2->startY = current->startY;
            break;
        default:
            break;
        }

        current->isPoint = false;
        current->centerX = center_of_mass(current->centerX, current->mass, x, mass);
        current->centerY = center_of_mass(current->centerY, current->mass, y, mass);
        current->mass += mass;

        return current;

    }else{
        current->centerX = center_of_mass(current->centerX, current->mass, x, mass);
        current->centerY = center_of_mass(current->centerY, current->mass, y, mass);
        current->mass += mass;

        switch(node_position(current->startX, current->startY, current->centerX, current->centerY, current->depth + 1)){
        case 1:
            if(current->upLeft == nullptr){
                int depth = current->depth + 1;
                double startX = current->startX;
                double startY = current->startY + size / pow(2, depth);
                current->upLeft = create_node(x, y, mass, depth, startX, startY);
            }else
                return insert_helper(x, y, mass, current->upLeft);
        case 2:
            if(current->downLeft != nullptr){
                int depth = current->depth + 1;
                double startX = current->startX;
                double startY = current->startY;
                current->downLeft = create_node(x, y, mass, depth, startX, startY);
            }else
                return insert_helper(x, y, mass, current->downLeft);
        case 3:
            if(current->upRight != nullptr){
                int depth = current->depth + 1;
                double startX = current->startX + size / pow(2, depth);
                double startY = current->startY + size / pow(2, depth);
                current->upRight = create_node(x, y, mass, depth, startX, startY);
            }else
                return insert_helper(x, y, mass, current->upRight);

        case 4:
            if(current->downRight != nullptr){
                int depth = current->depth + 1;
                double startX = current->startX;
                double startY = current->startY;
                current->downRight = create_node(x, y, mass, depth, startX, startY);
            }else
                return insert_helper(x, y, mass, current->downRight);
        default:
            return nullptr;
        }
    }
}