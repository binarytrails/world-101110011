#include "Vector.h"

Vector::Vector(const Vector& orig) {
    x = orig.x;
    y = orig.y;
    z = orig.z;
}

double Vector::magnitude () {
    return sqrt((x*x) + (y*y) + (z*z));
}

Vector Vector::normalize() {
    float mag = magnitude();
    return Vector(x/mag, y/mag, z/mag);
}

Vector Vector::negative() {
    return Vector(-x, -y, -z);
}

float Vector::dotProduct(Vector v) {
    return x*v.x + y*v.y + z*v.z;
}

Vector Vector::crossProduct(Vector v) {
    return Vector(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
}

Vector Vector::add(Vector v) {
    return Vector(x+v.x, y+v.y, z+v.z);
}

Vector Vector::mult(float scalar) {
    return Vector(x*scalar, y*scalar, z*scalar);
}