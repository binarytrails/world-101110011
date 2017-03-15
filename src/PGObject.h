#ifndef PGOBJECT_H
#define PGOBJECT_H

#include <vector>
#include <ctime>
#include <iostream>
#include <random>
#include "GLObject.h"

class PGObject {
public:
    std::vector<GLObject*> objects;
protected:
    float randNum(float min = -0.05f, float max = 0.05f);
};

#endif /* PGOBJECT_H */

