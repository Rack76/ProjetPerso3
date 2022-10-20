#ifndef PLANE_H
#define PLANE_H

#include "glm/glm.hpp"
#include "glm/ext.hpp"

class Plane
{
public:
	Plane() {
		normal = glm::vec3(0.0f, 1.0f, 0.0f);
	}
	bool isBelow(glm::vec3 point);
	glm::vec3 normal;
	float distance;
};

#endif