#ifndef GLOBJECT_H
#define GLOBJECT_H

#include <GLFW/glfw3.h>
#include <vector>

#include "Point.h"

class GLObject {
public:
    GLfloat* vertices;
    GLuint* indices;
    
    int vSize;
    int iSize;
    int iMax = 0;
    Point offset = Point(0,0,0);
    
    GLObject() : vSize(0), iSize(0) {};
    GLObject(int vSize, int iSize) : vSize(vSize), iSize(iSize) {};
    
    //Build a GLObject by combining vertices and indices of every objects in a vector
    GLObject(const std::vector<GLObject*>& others);
    
    GLObject(const GLObject& orig);
    
    //Two points used to form a hitbox
    bool sizeFirst = true;
    float sizeMin[3] = {0, 0, 0};
    float sizeMax[3] = {0, 0, 0};
    
    GLObject add(GLObject* other);
    
    virtual ~GLObject();
protected:
    int vCursor = 0;
    int iCursor = 0;
    void addPoint(float x, float y, float z);
    void addindice(int p1, int p2, int p3);
};

#endif /* GLOBJECT_H */

