#ifndef AABB_H
#define AABB_H

#include "glm/glm.hpp"
#include "glm/ext.hpp"

class AABB
{
public:
	glm::vec3 bottomLeftCorner;
	glm::vec3 topRightCorner;
	glm::vec3 center;
};

#endif