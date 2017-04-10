/*
Created by: Justin Velicogna.

*/

#ifndef SHADOW_H
#define SHADOW_H

#include "GLObject.h"
#include "..\glm\glm.hpp"
#include "Point.h"
#include <vector>


class Shadow : public GLObject{
	public:
		Shadow(GLObject orig, Point plane, glm::vec3 light): GLObject(orig.vSize,orig.iSize){
			for (int i = 0; i < orig.vSize; i+=3)
			{
				glm::vec3 point;
				if (intersect(light, glm::vec3(orig.vertices[i],orig.vertices[i+1],orig.vertices[i+2]), plane, point))
				{
					addPoint(point.x,point.y,point.z);
				}
				else
				{
					iSize = 0;
					vSize = 0;
					return;
				}
			}
			for (int i = 0; i < orig.iSize; i++)
			{
				indices[i] = orig.indices[i];
			}
			
		};
		Shadow(GLObject orig, Point plane);//just places light at 0,100,0
	private:
		bool intersect(glm::vec3 light, glm::vec3 point, Point plane, glm::vec3 &intersect);

};




#endif