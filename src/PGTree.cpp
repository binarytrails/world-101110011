#include "PGTree.h"

// TODO: Poorly coded
PGTree::PGTree(Point offset, float radius) {
    /*int height = (int)randNum(2.0f,5.0f);
    
    //Generate trunk
    float T[6+(height*3)] = {
        0.4f + randNum(), 0.0f, 0.0f,
        0.3f + randNum(), 0.2f, 0.0f
    };
    
    for (int c=0; c < height; c++) {
        T[c*3+6] = 0.2f + randNum();
        T[c*3+7] = 0.4f+0.2f*c;
        T[c*3+8] = 0.0f;
    }
        
    this->objects.push_back(new RotationalObject(height+2, 6, T, offset));*/
    
    lNode* r = new lNode(offset.direction);
    lNode* a = new lNode(Vector(-1,0.5,0));
    lNode* b = new lNode(Vector(1,0.5,0));
    lNode* c = new lNode(Vector(0,0.5,-1));
    lNode* d = new lNode(Vector(0,0.5,1));
    
    a->nodes.push_back(a);
    a->nodes.push_back(b);
    a->nodes.push_back(c);
    a->nodes.push_back(d);
    
    b->nodes.push_back(a);
    b->nodes.push_back(b);
    b->nodes.push_back(c);
    b->nodes.push_back(d);
    
    c->nodes.push_back(c);
    c->nodes.push_back(b);
    
    d->nodes.push_back(d);
    d->nodes.push_back(a);
    
    r->nodes.push_back(a);
    r->nodes.push_back(b);
    
    float T[6] = {
        0.3, 0.0f, 0.0f,
        0.3, 0.2f, 0.0f
    };
    
    /*RotationalObject* rot = new RotationalObject(2, 6, T, offset, Vector(0,1,0));
    this->objects.push_back(rot);
    rot = new RotationalObject(2, 6, T, rot->offset, Vector(1,1,0));
    this->objects.push_back(rot);
    rot = new RotationalObject(2, 6, T, rot->offset, Vector(1,1,0));
    this->objects.push_back(rot);*/
    
    loop(r, offset, 0.05, 0.01);
}

void PGTree::loop(lNode* rootNode, Point offset, float limit, float step) {
    if(limit >= step) {
        float P[6] = {
            limit, 0.0f, 0.0f,
            limit-step > 0 ? limit : 0, 0.2f, 0.0f
        };
        
        //std::cout << limit << "--- ";
        RotationalObject* rot = new RotationalObject(2, 6, P, offset, rootNode->direction);
        this->objects.push_back(rot);
        
        for(int c = 0; c < rootNode->nodes.size(); c++)
            loop(rootNode->nodes.at(c), rot->offset, limit-step, step);
    }
}
