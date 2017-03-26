#include "PGTree.h"

// TODO: Poorly coded
PGTree::PGTree(float offset[], float radius) {
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
    
    loop(offset, 0.5);
}

void PGTree::loop(float offset[], float limit) {
    if(limit > 0) {
        float T[9] = {
            limit, 0.0f, 0.0f,
            limit, 0.1f, 0.0f,
            limit-0.05f, 0.1f, 0.0f
        };
        
        this->objects.push_back(new RotationalObject(3, 6, T, offset));
        this->objects.push_back(new RotationalObject(2, 6, T, offset));
        offset[1] += 0.05f;
        
        loop(offset, limit-0.05f);
    }
}
