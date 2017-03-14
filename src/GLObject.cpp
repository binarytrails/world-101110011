#include "GLObject.h"

GLObject::~GLObject() {
    delete[] vertices;
    delete[] indices;
}

