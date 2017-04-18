/*
Created by: Justin Velicogna.

*/

#ifndef SHADOW_H
#define SHADOW_H

#include "GLObject.h"
#include <glm\glm.hpp>
#include "Point.h"
#include <vector>


class Shadow : public GLObject{
	public:
		Shadow(GLObject orig, glm::vec3 translation, Point plane, glm::vec3 light): GLObject(orig.vSize,orig.iSize){
			std::vector<float> points;
			
			for (int i = 0; i < orig.vSize; i+=3)
			{
				glm::vec3 point;
				if (intersect(light, glm::vec3(orig.vertices[i],orig.vertices[i+1],orig.vertices[i+2])+translation, plane, point))
				{
					points.push_back(point.x);
					points.push_back(point.y);
					points.push_back(point.z);

				}
				else
				{
					
					iSize = 0;
					vSize = 0;
					return;
				}
			}
			//---
			offset = orig.offset;
			this->vSize = orig.vSize;
			this->iSize = orig.iSize;
			vertices = new GLfloat[orig.vSize];
			colors = new GLfloat[orig.vSize];
			normals = new GLfloat[orig.vSize];
			indices = new GLuint[orig.iSize];
			//---
			for (int i = 0; i < orig.vSize; i++)
			{
				vertices[i] = points[i];
				colors[i] = 0.0;
				if (i % 3 == 0)
					normals[i] = plane.direction.x;
				if (i % 3 == 1)
					normals[i] = plane.direction.y;
				if (i % 3 == 2)
					normals[i] = plane.direction.z;

			}
			for (int i = 0; i < orig.iSize; i++)
			{
				indices[i] = orig.indices[i];
			}
			
		}





		Shadow(GLObject orig, glm::vec3 traslation, Point plane) : Shadow(orig, traslation, plane, glm::vec3(0, 100, 0)){}//just places light at 0,100,0
	private:
		const float shadowoffset = 0.001;//offset from shadow to ground
		bool intersect(glm::vec3 light, glm::vec3 point, Point plane, glm::vec3 &intersect);

};




#endif