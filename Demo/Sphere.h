#ifndef SPHERE_H
#define SPHERE_H

#include "glm/glm.hpp"
#include "glm/ext.hpp"

class Sphere
{
public:
	Sphere() {

	}
	Sphere(glm::vec3 center, float radius){
		m_center = center;
		m_radius = radius;
	}
	glm::vec3 m_center;
	float m_radius;
};

#endif