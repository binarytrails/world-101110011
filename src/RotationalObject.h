#ifndef ROTATIONALOBJECT_H
#define ROTATIONALOBJECT_H

#include <iostream>
#include "GLObject.h"
#include "Vector.h"

class RotationalObject : public GLObject {
public:
    RotationalObject(int PCount, int TCount, float P[], Point off = Point(), Vector axis = Vector(0,1,0));
    virtual ~RotationalObject();
private:

};

#endif /* ROTATIONALOBJECT_H */

