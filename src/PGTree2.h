#ifndef PGTREE2_H
#define PGTREE2_H

#include "PGObject.h"
#include "PGTree.h"
#include "GLObject.h"
#include "RotationalObject.h"
#include "Vector.h"

class PGTree2 : public PGTree {
public:
    PGTree2(Point offset, float radius = 0.05f, float steps = 0.01f, float height = 0.5f);
};

#endif /* PGTREE2_H */

