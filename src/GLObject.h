#ifndef GLOBJECT_H
#define GLOBJECT_H

#include <GLFW/glfw3.h>

class GLObject {
public:
    GLfloat* vertices;
    GLuint* indices;
    
    int vSize;
    int iSize;
    
    //Two points used to form a hitbox
    bool sizeFirst = true;
    float sizeMin[3] = {0, 0, 0};
    float sizeMax[3] = {0, 0, 0};
    
    virtual ~GLObject();
protected:
    int vCursor = 0;
    int iCursor = 0;
    void addPoint(float x, float y, float z);
    void addindice(int p1, int p2, int p3);
};

#endif /* GLOBJECT_H */

