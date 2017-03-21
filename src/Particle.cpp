#include "Particle.h"


Particle::Particle() {}

Particle::Particle(GLfloat x, GLfloat y, GLfloat z) {
	_x = x;
	_y = y;
	_z = z;
	_speedX = 0.0f;
	_speedY = 0.0f;
	_speedZ = 0.0f;
	_acceleration = -0.00001f;
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

void Particle::increment() {
	_speedY += _acceleration;
	_y += _speedY;
}

bool Particle::checkLife() {
	if (_y <= floor) return false;
}