#include "RotationalObject.h"
#include "Helper.h"

#include <iostream>

RotationalObject::RotationalObject(int PCount, int TCount, float P[], int offset[]) {
    if(!offset) offset = new int[3]();
    this->vSize = PCount * TCount * 3;
    this->iSize = (PCount-1) * TCount * 6;
    vertices = new GLfloat[this->vSize];
    indices = new GLuint[this->iSize];
    
    // Counters
    int VCount = 0;
    int ICount = 0;

    // Put every vertices (translations applied) and indices in their respective array
    float tranBot = 0.0f; //Bottom Transformation
    for(int ct = 0; ct < TCount; ct++){
        float tranTop = tranBot + (360 / (float)TCount); //Top Transformation

        for(int cp = 0; cp < PCount; cp++){
            //create vertices
            this->vertices[VCount] = Helper::getCos(tranBot)*P[(cp*3)+0] + Helper::getSin(tranBot)*P[(cp*3)+2] + offset[0];
            this->vertices[VCount+1] = P[(cp*3)+1] + offset[1];
            this->vertices[VCount+2] = Helper::getCos(tranBot)*P[(cp*3)+2] - Helper::getSin(tranBot)*P[(cp*3)+0] + offset[2];
            VCount += 3;

            //create indices
            if( cp < PCount-1 ) {
                int next = ct < TCount-1 ? (ct+1) : 0;
                this->indices[ICount]   = (ct*PCount)+cp;
                this->indices[ICount+1] = (ct*PCount)+cp+1;
                this->indices[ICount+2] = (next*PCount)+cp;
                this->indices[ICount+3] = (next*PCount)+cp+1;
                this->indices[ICount+4] = (ct*PCount)+cp+1;
                this->indices[ICount+5] = (next*PCount)+cp;
                ICount += 6;
            }
        }
        tranBot = tranTop;
    }
}

RotationalObject::~RotationalObject() {
}

