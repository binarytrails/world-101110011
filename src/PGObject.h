#ifndef PGOBJECT_H
#define PGOBJECT_H

#include <vector>
#include <ctime>
#include <iostream>
#include <random>
#include "GLObject.h"
#include "Vector.h"
#include "lNode.h"

class PGObject {
public:
    const float accuracy = 0.001; //used to exclude inaccuracy in low value computation errors
    std::vector<GLObject*> objects;
protected:
    float randNum(float min = -0.05f, float max = 0.05f);
    virtual void loop(lNode* rootNode, Point offset, float height, float limit, float step) = 0;
};

#endif /* PGOBJECT_H */

