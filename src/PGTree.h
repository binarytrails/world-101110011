#ifndef PGTREE_H
#define PGTREE_H

#include "PGObject.h"
#include "GLObject.h"
#include "RotationalObject.h"
#include "Vector.h"

class PGTree : public PGObject {
protected:
    virtual void loop(lNode* rootNode, Point offset, float height, float limit, float step);
};

#endif /* PGTREE_H */

