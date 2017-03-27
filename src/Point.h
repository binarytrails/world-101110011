/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Point.h
 * Author: sami
 *
 * Created on March 26, 2017, 6:09 PM
 */

#ifndef POINT_H
#define POINT_H

#include "Vector.h"


class Point {
public:
    float x, y, z;
    Vector direction;
    
    Point() : x(0), y(0), z(0), direction(Vector(0,0,0)) {};
    Point(float x, float y, float z) : x(x), y(y), z(z), direction(Vector(0,0,0)) {};
    Point(float x, float y, float z, Vector direction) : x(x), y(y), z(z), direction(direction) {};
    
    Point(const Point& orig);
    virtual ~Point();
    
    Point translate(Vector v);
    Point rotate(float sin, float cos, Vector axis);
    Point rotateY(float sin, float cos);
    Point add(Point p);
    Point mult(Point p);
private:

};

#endif /* POINT_H */

