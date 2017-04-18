#include "RotationalObject.h"
#include "Helper.h"
#include "Point.h"

#include <iostream>
#include <complex>

RotationalObject::RotationalObject(int PCount, int TCount, float P[], Point off, Vector axis) {
    offset = off;
    this->vSize = PCount * TCount * 3;
    this->iSize = (PCount-1) * TCount * 6;
    vertices = new GLfloat[this->vSize];
    colors = new GLfloat[this->vSize];
    normals = new GLfloat[this->vSize];
    indices = new GLuint[this->iSize];
    
    Vector axisN = axis.normalize();
    Vector yAxis(0,1,0);
    
    float offsetCos = (offset.direction.dotProduct(yAxis))/(offset.direction.magnitude() * yAxis.magnitude());
    float offsetSin = (offset.direction.crossProduct(yAxis)).magnitude()/(offset.direction.magnitude() * yAxis.magnitude());
    
    float axisCos = (axisN.dotProduct(yAxis))/(axisN.magnitude() * yAxis.magnitude());
    float axisSin = (axisN.crossProduct(yAxis)).magnitude()/(axisN.magnitude() * yAxis.magnitude());

    // Put every vertices (translations applied) and indices in their respective array
    float tranBot = 0.0f; //Bottom Transformation
    for(int ct = 0; ct < TCount; ct++){
        float tranTop = tranBot + (360 / (float)TCount); //Top Transformation

        float cos = Helper::getCos(tranBot);
        float sin = Helper::getSin(tranBot);
        
        for(int cp = 0; cp < PCount; cp++){
            //create points
            Point p(P[(cp*3)+0], P[(cp*3)+1], P[(cp*3)+2]);
            p = p.rotateY(sin, cos);
            p = p.rotate(offsetSin, offsetCos, offset.direction);
            p = p.rotate(axisSin, axisCos, axisN);
            p = p.translate(Vector(offset.x, offset.y, offset.z));
            
            this->addPoint(p.x,p.y,p.z,0,0,0,0.552, 0.541, 0.486);

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
    
    float yMax = 0;
    for(int cp = 0; cp < PCount*3; cp += 3) {
        if(P[cp+1] > yMax)
            yMax = P[cp+1];
    }
    
    Point newOffset(0,yMax,0);
    newOffset = newOffset.rotate(offsetSin, offsetCos, offset.direction);
    newOffset = newOffset.rotate(axisSin, axisCos, axisN);
    newOffset = newOffset.translate(Vector(offset.x, offset.y, offset.z));

    newOffset.direction = axis.add(offset.direction).normalize();
    offset = newOffset;
}

RotationalObject::~RotationalObject() {
}

