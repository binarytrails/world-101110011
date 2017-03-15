#include "PGTree.h"

// TODO: Poorly coded
PGTree::PGTree(float offset[]) {
    int height = (int)randNum(3.0f,6.0f);
    
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
        
    this->objects.push_back(new RotationalObject(height+2, 6, T, offset));
    
    //Generate leaves
    float L[9*3] = {
        0.0f, (height+2)*0.2f, 0.0f,
        0.4f, (height+2)*0.2f, 0.0f,
        0.5f + randNum(), (height+2)*0.2f+1*0.15f, 0.0f,
        0.6f + randNum(), (height+2)*0.2f+2*0.15f, 0.0f,
        0.8f + randNum(), (height+2)*0.2f+3*0.15f, 0.0f,
        0.8f + randNum(), (height+2)*0.2f+4*0.15f, 0.0f,
        0.6f + randNum(), (height+2)*0.2f+5*0.15f, 0.0f,
        0.4f + randNum(), (height+2)*0.2f+6*0.15f, 0.0f,
        0.0f, (height+2)*0.2f+7*0.15f, 0.0f
    };
    this->objects.push_back(new RotationalObject(9, 6, L, offset));
}

