#pragma once
#include "..\glew\glew.h"	// include GL Extension Wrangler
#include "..\glfw\glfw3.h"	// include GLFW helper library

//use GLM for transformation math
#include "..\glm\glm.hpp"   
#include "..\glm\gtc\matrix_transform.hpp"
#include "..\glm\gtc\type_ptr.hpp"

#include "Particle.h"


class Wind {

public:

	//defualt constructor
	Wind();

	//create a wind area with a vec3 denoting the direction and intensity
	Wind(GLfloat x, GLfloat y, GLfloat z);

	GLfloat getX();
	GLfloat getY();
	GLfloat getZ();

	void setX(GLfloat val);
	void setY(GLfloat val);
	void setZ(GLfloat val);

	// return the vector denoting the wind's direction/intensity
	glm::vec3 getWind();

private:

	glm::vec3 _direction;

};
