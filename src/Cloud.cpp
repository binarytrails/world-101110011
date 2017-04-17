#include "Cloud.h"

Cloud::Cloud() {}


Cloud::Cloud(GLfloat x, GLfloat y, GLfloat z, GLfloat wid, GLfloat len, GLuint dropCount, Wind *weather, GLuint type) {
	_x = x;
	_y = y;
	_z = z;
	_width = wid;
	_length = len;
	_dropCount = dropCount;
	_type = type;
	wind = weather;

	for (int i = 0; i < _dropCount; i++) {

		if (!isSeeded) seed();

		if (_type == 1) drops[i] = new Particle(randomBetween(x, x + wid), y, randomBetween(z, z + len));

       // printf("cloud : drop (%f, %f, %f)\n",
              //  drops[i]->getPosX(), drops[i]->getPosY(), drops[i]->getPosZ());
	}

	//initialize drops and link shaders
	setAllDrops();
	this->shader = new Shader("src/shaders/shaderRain.vs", "src/shaders/shaderRain.frag");

}

float Cloud::randomBetween(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}


void Cloud::increment() {
	for (int i = 0; i < _dropCount; i++) {
		drops[i]->increment();
	}
}


void Cloud::newDrop(int i) {
	if (!isSeeded) seed();
	if (_type == 1) drops[i] = new Particle(randomBetween(_x, _x + _width), _y, randomBetween(_z, _z + _length));
	drops[i]->setVelocity(drops[i]->getVelocity() + wind->getWind());
}


Particle Cloud::getDrop(int i) {
	return *drops[i];
}


int Cloud::setDrop(int k, int i) {
	allVertices[k++] = drops[i]->getPosX();
	allVertices[k++] = drops[i]->getPosY();
	allVertices[k++] = drops[i]->getPosZ();
	allVertices[k++] = drops[i]->getPosX();
	allVertices[k++] = drops[i]->getPosY() + 0.1f;
	allVertices[k++] = drops[i]->getPosZ();

	return k;
}


void Cloud::setAllDrops() {
	int k = 0;
	for (int i = 0; i < _dropCount; i++) {
		k = setDrop(k, i);
	}
}


GLfloat* Cloud::getAllDrops() {
	return allVertices;
}


void Cloud::updateAll() {

	int k = 0;
	for (int i = 0; i <  _dropCount; i++) {

		drops[i]->increment();

		if (drops[i]->checkLife() == false) {
			newDrop(i);
		}

		k = setDrop(k, i);
	}

}


void Cloud::initBuffers() {
	//generate the vertex buffer object(s), vertex array object
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(this->getAllDrops()), this->getAllDrops(), GL_DYNAMIC_DRAW);

	// Position attribute of the lines (location, # of float that make up a vertex (x,y,z), form (float, int, etc), normalised, distance between the starting point of each group of vertices, beginning offset) 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Unbind VAO
	glBindVertexArray(0);
}


GLenum Cloud::getRenderMode() const {
	return this->renderMode;
}

void Cloud::setRenderMode(const GLenum renderMode) {
	this->renderMode = renderMode;
}


void Cloud::render(const Window* window, const Camera* camera, const glm::mat4 view, const glm::mat4 projection) {
	this->shader->use();
	this->mvp(view, projection);
	this->draw();
}


void Cloud::rotate(const glm::vec3 spin) {
	this->model = glm::rotate(this->model, spin.x, glm::vec3(1, 0, 0));
	this->model = glm::rotate(this->model, spin.y, glm::vec3(0, 1, 0));
	this->model = glm::rotate(this->model, spin.z, glm::vec3(0, 0, 1));
}


void Cloud::mvp(glm::mat4 view, glm::mat4 projection) {

	// Get their uniform location
	GLint modelLoc = glGetUniformLocation(this->shader->programId, "model");
	GLint viewLoc = glGetUniformLocation(this->shader->programId, "view");
	GLint projLoc = glGetUniformLocation(this->shader->programId, "projection");

	// Set the mdoel, view, projection matrixes each frame
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(this->model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void Cloud::draw() {
	//update all the drops positions
	this->updateAll();
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 6000, this->getAllDrops(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//bind the VAO
	glBindVertexArray(VAO);

	//draw all the drops
	glDrawArrays(GL_LINES, 0, 6000);

	//unbind VAO
	glBindVertexArray(0);
}


void Cloud::seed()
{
	srand((unsigned)time(NULL));
	isSeeded = true;
}
