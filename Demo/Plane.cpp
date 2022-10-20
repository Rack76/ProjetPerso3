#include "Plane.h"

bool Plane::isBelow(glm::vec3 point)
{
	float dotProduct = glm::dot(normal, point);
	return dotProduct > distance ? true : false;
}