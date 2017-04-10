#ifndef GLOBJECT_H
#define GLOBJECT_H

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <complex.h>

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
    GLuint VBO, instanceVBO, VAO, EBO;
    int instanceCount = -1;
    
    GLObject() : vSize(0), iSize(0) {};
    GLObject(int vSize, int iSize) : vSize(vSize), iSize(iSize) {};
    GLObject(int vSize, int iSize, GLfloat* v, GLuint* i);
    
    //Build a GLObject by combining vertices and indices of every objects in a vector
    GLObject(const std::vector<GLObject*>& others);
    
    GLObject(const GLObject& orig);
    
    //Two points used to form a hitbox
    bool sizeFirst = true;
    float sizeMin[3] = {0, 0, 0};
    float sizeMax[3] = {0, 0, 0};
    
    GLObject add(GLObject* other);
    void draw(glm::vec3* translations, int count);
    
    virtual ~GLObject();
protected:
    int vCursor = 0;
    int iCursor = 0;
    void addPoint(float x, float y, float z);
    void addindice(int p1, int p2, int p3);
	void addTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);//assume color data is provided in vectors
	
private:
	glm::vec3 getNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
};

#endif /* GLOBJECT_H */

