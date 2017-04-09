#include "Wind.h"

Wind::Wind() {}

Wind::Wind(GLfloat x, GLfloat y, GLfloat z) {
	_direction = glm::vec3(x, y, z);
}



GLfloat Wind::getX() {
	return _direction.x;
}

GLfloat Wind::getY() {
	return _direction.y;
}

GLfloat Wind::getZ() {
	return _direction.z;
}



void Wind::setX(GLfloat val) {
	_direction.x = val;
}

void Wind::setY(GLfloat val) {
	_direction.y = val;
}

void Wind::setZ(GLfloat val) {
	_direction.z = val;
}



glm::vec3 Wind::getWind() {
	return _direction;
}