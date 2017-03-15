#ifndef ROTATIONALOBJECT_H
#define ROTATIONALOBJECT_H

#include "GLObject.h"

class RotationalObject : public GLObject {
public:
    RotationalObject(int PCount, int TCount, float P[], float offset[] = nullptr);
    virtual ~RotationalObject();
private:

};

#endif /* ROTATIONALOBJECT_H */

