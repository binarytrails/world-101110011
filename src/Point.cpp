/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Point.cpp
 * Author: sami
 * 
 * Created on March 26, 2017, 6:09 PM
 */

#include "Point.h"

Point::Point(const Point& orig) {
    x = orig.x;
    y = orig.y;
    z = orig.z;
    direction = orig.direction;
}

Point::~Point() {
}

Point Point::translate(Vector v) {
    return Point(x+v.x, y+v.y, z+v.z, direction);
}
Point Point::rotate(float sin, float cos, Vector axis) {
    float xNew = (cos+axis.x*axis.x*(1-cos))*x    + (axis.x*axis.y*(1-cos)-axis.z*sin)*y + (axis.x*axis.z*(1-cos)+axis.y*sin)*z;
    float yNew = (axis.y*axis.x*(1-cos)+axis.z*sin)*x + (cos+axis.y*axis.y*(1-cos))*y    + (axis.y*axis.z*(1-cos)-axis.x*sin)*z;
    float zNew = (axis.z*axis.x*(1-cos)-axis.y*sin)*x + (axis.z*axis.y*(1-cos)+axis.x*sin)*y + (cos+axis.z*axis.z*(1-cos))*z;
    return Point(xNew, yNew, zNew, direction);
}
Point Point::rotateY(float sin, float cos){
    return Point(cos*x + sin*z, y, cos*z - sin*x, direction);
}


Point Point::add(Point p) {
    return Point(x+p.x,y+p.y,z+p.z, direction);
}
Point Point::mult(Point p) {
    return Point(x*p.x,y*p.y,z*p.z, direction);
}