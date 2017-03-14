#ifndef GLOBJECT_H
#define GLOBJECT_H

#include <GLFW/glfw3.h>

class GLObject {
public:
    GLfloat* vertices;
    GLuint* indices;
    
    int vSize;
    int iSize;
    
    virtual ~GLObject();
private:

};

#endif /* GLOBJECT_H */

