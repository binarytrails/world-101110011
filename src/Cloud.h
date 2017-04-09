#pragma once
#include "..\glew\glew.h"	// include GL Extension Wrangler
#include "..\glfw\glfw3.h"	// include GLFW helper library

//use GLM for transformation math
#include "..\glm\glm.hpp"   
#include "..\glm\gtc\matrix_transform.hpp"
#include "..\glm\gtc\type_ptr.hpp"

#include "Particle.h"
#include "Wind.h"
#include <vector>
#include <time.h>

class Cloud {

public:

	//defualt constructor
	Cloud();

	//create a cloud at a certain location with certain dimensions, and the number of Particles it creates
	Cloud(GLfloat x, GLfloat y, GLfloat z, GLuint wid, GLuint len, GLuint dropCount, Wind *weather);

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

	void seed();


private:
	GLfloat _x, _y, _z;
	GLfloat _speedX, _speedZ;
	GLuint _width, _length;

	GLuint _dropCount;
	Particle *drops[1000];
	GLfloat allVertices[6000];

	Wind *wind;

	bool isSeeded = false;
};