#ifndef PHYSICS_OBJECT_H
#define PHYSICS_OBJECT_H

#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <vector>
#include "FaceInfo.h"
#include "EdgeInfo.h"
#include "BVH.h"
#include "AdaptiveTimeStepSolver.h"

class PhysicsObject
{
public:
	PhysicsObject() {

	}
	PhysicsObject(std::vector<FaceInfo> faceInfoList, std::vector<EdgeInfo> edgeInfoList, std::vector<glm::vec3> normals
		, float mass, float cof, float cor
		, glm::vec3 position
		, glm::mat4 orientation, 
		std::vector<glm::vec3> vertices)
	{
		m_faceInfoList = faceInfoList;
		m_edgeInfoList = edgeInfoList;
		m_normals = normals;
		m_mass = mass;
		m_cof = cof;
		m_cor = cor;
		m_position = glm::vec4(position, 1.0);
		m_orientation = orientation;
		m_vertices = vertices;
	}
	void move(float dt);

	glm::vec4 m_position;
	glm::vec4 linearVelocity;
	glm::vec4 linearAcceleration;
	glm::vec4 centerOfMass;
	glm::vec4 netForce;
	glm::mat4 netTorque;
	glm::mat4 m_orientation;
	glm::mat4 angularVelocity;
	glm::mat4 angularAcceleration;
	glm::mat4 inertiaTensor;
	glm::mat4 localFrame;
	std::vector<glm::vec3> m_vertices;
	std::vector<FaceInfo> m_faceInfoList;
	BVH bvh;
private:
	AdaptiveTimeStepSolver solver;
	std::vector<EdgeInfo> m_edgeInfoList;
	std::vector<glm::vec3> m_normals;
	float m_mass;
	float m_cof;
	float m_cor;

};

#endif