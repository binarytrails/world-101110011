#ifndef LNODE_H
#define LNODE_H

#include <vector>

#include "Vector.h"

/*
 * Store a l-system command and list of l-system vocabulary
 */

class lNode {
public:
    std::vector<lNode*> nodes;
    Vector direction;
    
    lNode() : direction(Vector(0,0,0)) {};
    lNode(Vector direction) : direction(direction) {};
    lNode(const lNode& orig);
    virtual ~lNode();
private:

};

#endif /* LNODE_H */

