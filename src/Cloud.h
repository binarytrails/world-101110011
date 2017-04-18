#pragma once
#include <vector>
#include <time.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//use GLM for transformation math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Wind.h"

#include <stdio.h>

class Cloud : public Mesh {

public:

	//defualt constructor
	Cloud();

	//create a cloud at a certain location with certain dimensions, and the number of Particles it creates
	Cloud(GLfloat x, GLfloat y, GLfloat z, GLfloat wid, GLfloat len, GLuint dropCount, Wind *weather, GLuint type);

	//generates random float in a range (used for setting drops initial position)
	float randomBetween(float a, float b);

	//applies effects of wind to a cloud's position/particles
	void increment();

	//create new drop when one has been deleted
	void newDrop(int i);

	//return a certain drop from the cloud
	Particle getDrop(int i);

	//create all the vertices for one drop
	int setDrop(int k, int i);

	//creates all the vertices needed for one frame of rain fall
	void setAllDrops();

	//return all the vertices for the drops
	GLfloat* getAllDrops();

	//update all the drops
	void updateAll();

	void initBuffers();

	GLenum getRenderMode() const;

	void setRenderMode(const GLenum renderMode);

	void render(const Window* window, const Camera* camera, const glm::mat4 view, const glm::mat4 projection);

	void rotate(const glm::vec3 spin);

	void mvp(glm::mat4 view, glm::mat4 projection);

	void draw();

	void deallocate();

	void seed();

	GLuint VBO, VAO, EBO;

private:
	GLfloat _x, _y, _z;
	GLfloat _speedX, _speedZ;
	GLfloat _width, _length;

	//the type of cloud: 1 for rain, 2 for snow
	GLuint _type;
	GLuint _dropCount;
	Particle* drops[1000];
	GLfloat allVertices[6000];
	GLfloat allVerticeSnow[12000];
	GLuint indices[6000];

	Shader *shader;
	Wind *wind;

	GLenum renderMode;
	glm::mat4 model;
	glm::mat4 view;

	bool isSeeded = false;
};
