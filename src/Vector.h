#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>


class Vector {
public:
    float x, y, z;
    Vector() : x(0), y(0), z(0) {};
    Vector(float x, float y, float z) : x(x), y(y), z(z) {};
    Vector(const Vector& orig);
    
    double magnitude ();
    Vector normalize();
    Vector negative();
    float dotProduct(Vector v);
    Vector crossProduct(Vector v);
    Vector add(Vector v);
    Vector mult(float scalar);
};

#endif /* VECTOR_H */

