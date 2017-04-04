#include "GLObject.h"

void GLObject::draw(glm::vec3* translations, int count) {
    if(this->instanceCount == -1) {
        instanceCount = count;
        
        // Store instance data in an array buffer
        glGenBuffers(1, &instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 100, &translations[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vSize*sizeof(vertices[0]), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSize*sizeof(indices[0]), indices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

        // Also set instance data
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);	
        glVertexAttribDivisor(2, 1); // Tell OpenGL this is an instanced vertex attribute.
    } else {
        glBindVertexArray(VAO);
    }
    
    glDrawElementsInstanced(GL_TRIANGLES, iSize, GL_UNSIGNED_INT, 0, instanceCount);
}

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