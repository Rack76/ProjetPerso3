#ifndef PHYSICS_OBJECT_H
#define PHYSICS_OBJECT_H

#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <vector>
#include "FaceInfo.h"
#include "EdgeInfo.h"
#include "BVH.h"

class PhysicsObject
{
public:
	PhysicsObject()
	{

	}
	PhysicsObject(std::vector<FaceInfo> faceInfoList, std::vector<EdgeInfo> edgeInfoList
		, float mass, float cof, float cor
		, glm::vec3 position
		, glm::mat4 orientation,  BVH bvh)
	{
		m_faceInfoList = faceInfoList;
		m_edgeInfoList = edgeInfoList;
		m_mass = mass;
		m_cof = cof;
		m_cor = cor;
		m_position = position;
		m_orientation = orientation;
		m_bvh = bvh;
	}

	glm::vec3 m_position;
	glm::mat4 m_orientation;
private:
	std::vector<FaceInfo> m_faceInfoList;
	std::vector<EdgeInfo> m_edgeInfoList;
	float m_mass;
	float m_cof;
	float m_cor;
	glm::mat3 InertiaTensor;
	BVH m_bvh;

};

#endif