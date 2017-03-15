#include "GLObject.h"

void GLObject::addPoint(float x, float y, float z) {
    this->vertices[vCursor] = x;
    this->vertices[vCursor+1] = y;
    this->vertices[vCursor+2] = z;
    
    for(int c = 0; c < 3; c++) {
        if(this->vertices[vCursor+c] > sizeMax[c] || sizeFirst)
            sizeMax[c] = this->vertices[vCursor+c];
        if(this->vertices[vCursor+c] < sizeMin[c] || sizeFirst)
            sizeMin[c] = this->vertices[vCursor+c];
    }
    
    sizeFirst = false;
    vCursor += 3;
}

void GLObject::addindice(int p1, int p2, int p3) {
    this->indices[iCursor]   = p1;
    this->indices[iCursor+1] = p2;
    this->indices[iCursor+2] = p3;
    
    iCursor += 3;
}

GLObject::~GLObject() {
    delete[] vertices;
    delete[] indices;
}

