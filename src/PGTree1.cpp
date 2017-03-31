#include "PGTree1.h"

PGTree1::PGTree1(Point offset, float radius) {
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
    r->nodes.push_back(c);
    r->nodes.push_back(d);
    
    float T[6] = {
        0.3, 0.0f, 0.0f,
        0.3, 0.2f, 0.0f
    };
    
    loop(r, offset, radius, 0.01);
}