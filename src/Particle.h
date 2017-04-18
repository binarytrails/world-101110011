#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//use GLM for transformation math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>

class Particle {

public:

	//defualt constructor
	Particle();

	//create a particle at a particular location
	Particle(GLfloat x, GLfloat y, GLfloat z);

	//getters
	GLfloat getPosX();
	GLfloat getPosY();
	GLfloat getPosZ();

	//return a particles current velocity
	glm::vec3 getVelocity();

	//set a particle's velocity
	void setVelocity(glm::vec3 wind);

	//applies effects of wind and gravity to a particle
	void increment();

	//checks if the particle is still alive, and kills it if it has expired
	bool checkLife();

	float randomBetween(float a, float b);


private:
	//position
	GLfloat _x, _y, _z;

	//velocity
	glm::vec3 velocity;

	//change in velocity each frame
	GLfloat _acceleration;

	//point where the particle dies
	GLfloat floor = 0.0f;
};
