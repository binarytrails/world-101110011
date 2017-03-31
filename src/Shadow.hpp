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
		Shadow(GLObject orig, Point plane, glm::vec3 light);
		Shadow(GLObject orig, Point plane); //assume light comes from above
	private:
		glm::vec3 Intersect(glm::vec3 light, glm::vec3 point, Point plane, glm::vec3 &intersect);

};




#endif