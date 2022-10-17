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
	PhysicsObject(std::vector<FaceInfo> faceInfoList, std::vector<EdgeInfo> edgeInfoList, std::vector<glm::vec3> normals
		, float mass, float cof, float cor
		, glm::vec3 position
		, glm::mat4 orientation)
	{
		m_faceInfoList = faceInfoList;
		m_edgeInfoList = edgeInfoList;
		m_normals = normals;
		m_mass = mass;
		m_cof = cof;
		m_cor = cor;
		m_position = glm::vec4(position, 1.0);
		m_orientation = orientation;
	}

	glm::vec4 m_position;
	glm::vec4 centerOfMass;
	glm::mat4 m_orientation;
	glm::mat4 localInertiaTensor;
	glm::mat4 localFrame;
private:
	std::vector<FaceInfo> m_faceInfoList;
	std::vector<EdgeInfo> m_edgeInfoList;
	std::vector<glm::vec3> m_normals;
	float m_mass;
	float m_cof;
	float m_cor;
	BVH bvh;

};

#endif