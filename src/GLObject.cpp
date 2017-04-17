#include "GLObject.h"

void GLObject::initBuffers(glm::vec3* translations, int count) {
    instanceCount = count;
    
    // Store instance data in an array buffer
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * instanceCount, &translations[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &colorVBO);
    glGenBuffers(1, &normalVBO);
    glGenBuffers(1, &EBO);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vSize*sizeof(vertices[0]), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSize*sizeof(indices[0]), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    //Normal
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    glBufferData(GL_ARRAY_BUFFER, vSize*sizeof(normals[0]), normals, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    //Color
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, vSize*sizeof(colors[0]), colors, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    // Also set instance data
    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);	
    glVertexAttribDivisor(3, 1); // Tell OpenGL this is an instanced vertex attribute.
    
    // unbind vbo
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // unbind vao
    glBindVertexArray(0);
}

void GLObject::draw() {
    glBindVertexArray(VAO);
    glDrawElementsInstanced(GL_TRIANGLES, iSize, GL_UNSIGNED_INT, 0, instanceCount);
    glBindVertexArray(0);
}

void GLObject::addPoint(float x, float y, float z, float nx, float ny, float nz, float r, float g, float b) {
    this->vertices[vCursor] = x;
    this->vertices[vCursor+1] = y;
    this->vertices[vCursor+2] = z;
    
    this->colors[vCursor] = r;
    this->colors[vCursor+1] = g;
    this->colors[vCursor+2] = b;
    
    this->normals[vCursor] = nx;
    this->normals[vCursor+1] = ny;
    this->normals[vCursor+2] = nz;
    
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

void GLObject::addTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3){
	//TODO: Make this work with colors and alpha data. Also add normal data.
	
	glm::vec3 normal = getNormal(p1, p2, p3);

	data.push_back(p1.x);
	data.push_back(p1.y);
	data.push_back(p1.z);
	data.push_back(normal.x);
	data.push_back(normal.y);
	data.push_back(normal.z);
	data.push_back(p1.r);
	data.push_back(p1.g);
	data.push_back(p1.b);
	data.push_back(p1.t); //I am pretty sure t is for transperancy

	data.push_back(p2.x);
	data.push_back(p2.y);
	data.push_back(p2.z);
	data.push_back(normal.x);
	data.push_back(normal.y);
	data.push_back(normal.z);
	data.push_back(p2.r);
	data.push_back(p2.g);
	data.push_back(p2.b);
	data.push_back(p2.t); //I am pretty sure t is for transperancy


	data.push_back(p3.x);
	data.push_back(p3.y);
	data.push_back(p3.z);
	data.push_back(normal.x);
	data.push_back(normal.y);
	data.push_back(normal.z);
	data.push_back(p3.r);
	data.push_back(p3.g);
	data.push_back(p3.b);
	data.push_back(p3.t); //I am pretty sure t is for transperancy



	//addPoint(p1.x, p1.y, p1.z);
	//addPoint(p2.x, p2.y, p2.z);
	//addPoint(p3.x, p3.y, p3.z);


	
	
	
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
    colors = new GLfloat[vSize];
    normals = new GLfloat[vSize];
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

glm::vec3 GLObject::getNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3){
	return glm::normalize(glm::cross(p2-p1,p3-p1));
	
}