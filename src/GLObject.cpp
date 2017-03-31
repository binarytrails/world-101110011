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
    
    if(p1 > iMax) iMax = p1;
    if(p2 > iMax) iMax = p2;
    if(p3 > iMax) iMax = p3;
    
    iCursor += 3;
}

GLObject::GLObject(int vSize, int iSize, GLfloat* v, GLuint* i) : vSize(vSize), iSize(iSize) {
    vertices = new GLfloat[vSize];
    indices = new GLuint[iSize];
    
    for(int c = 0; c < vSize; c+=3) {
        addPoint(v[c], v[c+1], v[c+2]);
    }
    
    for(int c = 0; c < iSize; c+=3) {
        addindice(i[c], i[c+1], i[c+2]);
    }
}

GLObject::~GLObject() {
    delete[] vertices;
    delete[] indices;
}

GLObject::GLObject(const GLObject& orig) {
    vertices = orig.vertices;
    indices = orig.indices;
    vSize = orig.vSize;
    iSize = orig.iSize;
    iMax = orig.iMax;
}

GLObject::GLObject(const std::vector<GLObject*>& others){
    vSize = 0;
    iSize = 0;
    
    for(int c = 0; c < others.size(); c++) {
        vSize += others.at(c)->vSize;
        iSize += others.at(c)->iSize;
    }
    
    vertices = new GLfloat[vSize];
    indices = new GLuint[iSize];
    
    int max = 0;
    for(int o = 0; o < others.size(); o++) {
        for(int c = 0; c < others.at(o)->vSize; c += 3)
            addPoint(others.at(o)->vertices[c], others.at(o)->vertices[c+1], others.at(o)->vertices[c+2]);
        for(int c = 0; c < others.at(o)->iSize; c += 3)
            addindice(others.at(o)->indices[c]+max, others.at(o)->indices[c+1]+max, others.at(o)->indices[c+2]+max);
        max += others.at(o)->iMax + 1;
    }
}