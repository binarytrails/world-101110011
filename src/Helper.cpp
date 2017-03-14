#include "Helper.h"
#define PI 3.14159265

Helper::Helper() {
}

Helper::~Helper() {
}

float Helper::getSin(float val) {
	return sin(val*PI/180);
}

float Helper::getCos(float val) {
	return cos(val*PI/180);
}