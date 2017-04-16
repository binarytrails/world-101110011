#include "PGTree2.h"

PGTree2::PGTree2(Point offset, float radius, float steps, float height) {
    lNode* r = new lNode(offset.direction);
    lNode* a = new lNode(Vector(0,1,0));
    
    lNode* b = new lNode(Vector(0,0.5,1));
    lNode* c = new lNode(Vector(0,0.5,-1));
    
    lNode* d = new lNode(Vector(1,1,0));
    lNode* e = new lNode(Vector(-1,1,0));
    
    a->nodes.push_back(a);
    a->nodes.push_back(b);
    a->nodes.push_back(c);
    
    b->nodes.push_back(b);
    b->nodes.push_back(d);
    b->nodes.push_back(e);
    
    c->nodes.push_back(c);
    c->nodes.push_back(d);
    c->nodes.push_back(e);
    
    r->nodes.push_back(a);
    r->nodes.push_back(b);
    r->nodes.push_back(c);
    
    float T[6] = {
        radius, 0.0f, 0.0f,
        radius, height, 0.0f
    };
    
    loop(r, offset, height, radius, steps);
}
