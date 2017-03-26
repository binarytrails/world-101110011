#ifndef PGTREE_H
#define PGTREE_H

#include "PGObject.h"
#include "GLObject.h"
#include "RotationalObject.h"
#include "Vector.h"

class PGTree : public PGObject {
public:
    PGTree(float offset[], float radius = 0.5);
protected:
    virtual void loop(float offset[], float limit);
};

#endif /* PGTREE_H */

