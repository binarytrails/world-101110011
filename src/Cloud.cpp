#include "Cloud.h"

Cloud::Cloud() {}


Cloud::Cloud(GLfloat x, GLfloat y, GLfloat z, GLuint wid, GLuint len, GLuint dropCount, Wind *weather) {
	_x = x;
	_y = y;
	_z = z;
	_width = wid;
	_length = len;
	_dropCount = dropCount;
	wind = weather;

	for (int i = 0; i < _dropCount; i++) {

		if (!isSeeded) seed();

		drops[i] = new Particle((float)(rand() % wid) + x, (float)(rand() % 10) + y, (float)(rand() % len) + z);
	}

}


void Cloud::increment() {
	for (int i = 0; i < _dropCount; i++) {
		drops[i]->increment();
	}
}


void Cloud::newDrop(int i) {
	if (!isSeeded) seed();
	drops[i] = new Particle((float)(rand() % _width) + _x, (float)(rand() % 10) + _y, (float)(rand() % _length) + _z);
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


void Cloud::seed()
{
	srand((unsigned)time(NULL));
	isSeeded = true;
}