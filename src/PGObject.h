#ifndef PGOBJECT_H
#define PGOBJECT_H

#include <vector>
#include <ctime>
#include <iostream>
#include <random>
#include "GLObject.h"
#include "Vector.h"

class PGObject {
public:
    std::vector<GLObject*> objects;
    std::vector<Vector*> vocabulary;
protected:
    float randNum(float min = -0.05f, float max = 0.05f);
    virtual void loop(float offset[], float limit) = 0;
};

#endif /* PGOBJECT_H */

