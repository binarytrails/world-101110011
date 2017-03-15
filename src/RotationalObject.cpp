#include "RotationalObject.h"
#include "Helper.h"

#include <iostream>

RotationalObject::RotationalObject(int PCount, int TCount, float P[], float offset[]) {
    if(!offset) offset = new float[3]();
    this->vSize = PCount * TCount * 3;
    this->iSize = (PCount-1) * TCount * 6;
    vertices = new GLfloat[this->vSize];
    indices = new GLuint[this->iSize];

    // Put every vertices (translations applied) and indices in their respective array
    float tranBot = 0.0f; //Bottom Transformation
    for(int ct = 0; ct < TCount; ct++){
        float tranTop = tranBot + (360 / (float)TCount); //Top Transformation

        for(int cp = 0; cp < PCount; cp++){
            //create points
            this->addPoint(
                    Helper::getCos(tranBot)*P[(cp*3)+0] + Helper::getSin(tranBot)*P[(cp*3)+2] + offset[0],
                    P[(cp*3)+1] + offset[1],
                    Helper::getCos(tranBot)*P[(cp*3)+2] - Helper::getSin(tranBot)*P[(cp*3)+0] + offset[2]
            );

            //create indices
            if( cp < PCount-1 ) {
                int next = ct < TCount-1 ? (ct+1) : 0;
                this->addindice(
                        (ct*PCount)+cp,
                        (ct*PCount)+cp+1,
                        (next*PCount)+cp
               );
                this->addindice(
                        (next*PCount)+cp+1,
                        (ct*PCount)+cp+1,
                        (next*PCount)+cp
               );
            }
        }
        tranBot = tranTop;
    }
}

RotationalObject::~RotationalObject() {
}

