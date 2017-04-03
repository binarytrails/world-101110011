/*
Created by: Justin Velicogna.

*/

#ifndef SHADOW_H
#define SHADOW_H

#include "GLObject.h"
#include "\glm\glm.hpp"
#include <vector>


class Shadow : public GLObject{
	public:
		Shadow(GLObject orig, Point plane, glm::vec3 light): GLObject(orig.vSize,orig.iSize){
			for (int i = 0; i < orig.vSize; i++)
			{
				glm::vec3 point;
				if (intersect(light, orig.vertices[i], plane, point))
				{
					vertices[i] = point;
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