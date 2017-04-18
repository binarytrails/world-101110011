#include "Particle.h"
#include <iostream>


Particle::Particle() {}

Particle::Particle(GLfloat x, GLfloat y, GLfloat z, GLuint mode) {
	_x = x;
	_y = y;
	_z = z;
	type = mode;
	velocity = glm::vec3(0.0f, randomBetween(0.0f, -0.005f), 0.0f);

	if (type == 1) {
		_acceleration = -0.00001f;
	}
	else if (type == 2) {
		_acceleration = -0.000001f;
	}
}

GLfloat Particle::getPosX() {
	return _x;
}

GLfloat Particle::getPosY() {
	return _y;
}

GLfloat Particle::getPosZ() {
	return _z;
}

glm::vec3 Particle::getVelocity() {
	return velocity;
}

void Particle::setVelocity(glm::vec3 v) {
	velocity = v;
}

void Particle::increment() {
	velocity.y += _acceleration;

	if (type == 1) {
		_x += velocity.x;
	}
	else if (type == 2) {
		switchCounter++;
		if (switchCounter % 999 > 450) {
			_x += 0.001;
		}
		else {
			_x -= 0.001;
		}
	}

	_y += velocity.y;
	_z += velocity.z;
}

bool Particle::checkLife() {
    //printf("particle : checklife : y(%f)\n", _y);
    if (_y <= floor)
        return false;
    return true;
}

float Particle::randomBetween(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}
