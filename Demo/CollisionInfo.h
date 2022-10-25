#ifndef COLLISION_INFO_H
#define COLLISION_INFO_H

enum {TRIANGLE_VERTEX, VERTEX_TRIANGLE, EDGE_EDGE};

#include "glm/glm.hpp"
#include "glm/ext.hpp"

class CollisionInfo
{
public:
	CollisionInfo(int handle0, int handle1, float _time)
	{
		object0 = handle0;
		object1 = handle1;
		time = _time;
	}
	bool operator==(const CollisionInfo& collisionInfo) const {
		if ((this->object0 == collisionInfo.object0) ||
			(this->object0 == collisionInfo.object1) ||
			(this->object1 == collisionInfo.object0) ||
			(this->object1 == collisionInfo.object1))
			return true;
		return false;
	}
	glm::vec3 normal;
	glm::vec3 point;
	float time;
	int collisionType;
	int object0;
	int object1;
};

#endif