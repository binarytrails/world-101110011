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
    GLfloat* colors;
    GLfloat* normals;
    GLuint* indices;

    std::vector<float> data; //easier to add too, no need to have a starting size
    int vSize;
    int iSize;
    int iMax = 0;
    Point offset = Point(0,0,0);
    GLuint VBO, instanceVBO, colorVBO, normalVBO, VAO, EBO;
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
    void addPoint(float x, float y, float z, float nx = 0, float ny = 0, float nz = 0, float r = 1.0f, float g = 1.0f, float b = 1.0f);
    void addindice(int p1, int p2, int p3);
    void addTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);//assume color data is provided in vectors
	
private:
	glm::vec3 getNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
};

#endif /* GLOBJECT_H */

