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

        printf("cloud : drop (%f, %f, %f)\n",
                drops[i]->getPosX(), drops[i]->getPosY(), drops[i]->getPosZ());
	}

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


void Cloud::seed()
{
	srand((unsigned)time(NULL));
	isSeeded = true;
}
