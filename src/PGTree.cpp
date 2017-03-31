#include "PGTree.h"

void PGTree::loop(lNode* rootNode, Point offset, float limit, float step) {
    if(limit >= step) {
        float P[6] = {
            limit, 0.0f, 0.0f,
            limit-step > 0 && rootNode->nodes.size() > 0 ? limit : 0, 0.2f, 0.0f
        };
        
        //std::cout << limit << "--- ";
        RotationalObject* rot = new RotationalObject(2, 6, P, offset, rootNode->direction);
        this->objects.push_back(rot);
        
        for(int c = 0; c < rootNode->nodes.size(); c++)
            loop(rootNode->nodes.at(c), rot->offset, limit-step, step);
    }
}
