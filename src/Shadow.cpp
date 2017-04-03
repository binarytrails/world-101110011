#include "Shadow.hpp"
#include "/glm/glm.hpp"



bool Shadow::intersect(glm::vec3 light, glm::vec3 point, Point slope, glm::vec3 &intersect){
	

	
	float D = -1 * (slope.x*slope.direction.x + slope.y*slope.direction.y + slope.z*slope.direction.z);

	float numerator = slope.direction.x*light.x + slope.direction.y*light.y + slope.direction.z*light.z + D;

	float divisor = slope.direction.x*point.x + slope.direction.y*point.y + slope.direction.z*point.z; //noot noot

	if (divisor == 0)
		return false; //plane is paralell to line. Return bad value


	intersect.x = light.x - point.x*numerator / divisor;
	intersect.y = light.y - point.y*numerator / divisor;
	intersect.z = light.z - point.z*numerator / divisor;
	return true;

}