#include "PhysicsEngine.h"

int PhysicsEngine::registerObject(std::vector<FaceInfo> faceInfoList, std::vector<EdgeInfo> edgeInfoList, std::vector<glm::vec3> normals,
									std::vector<glm::vec3> vertices
	                              ,float mass, float cof, float cor
								  ,glm::vec3 position
								  ,glm::mat4 orientation)
{
	int handle = handles.front();
	handles.pop();
	objects.emplace(handle, PhysicsObject(faceInfoList, edgeInfoList, normals, mass,
		cof, cor, position, orientation, vertices)).first;
	computeObjectExtendedRepresentation(handle);
	return handle;
}

void PhysicsEngine::mapObject(int handle, glm::vec3& position, glm::mat4& orientation)
{
	position = objects[handle].m_position;
	orientation = objects[handle].m_orientation;
}

void PhysicsEngine::deleteObject(int handle)
{
	objects.erase(handle);
	handles.push(handle);
}

void PhysicsEngine::computeObjectExtendedRepresentation(int handle)
{
	//objects[handle].centerOfMass = ;
	//objects[handle].localInertiaTensor = ;
	objects[handle].localFrame =  glm::translate(objects[handle].inertiaTensor, glm::vec3(objects[handle].centerOfMass));
	//objects[handle].bvh = ;
}

void PhysicsEngine::run()
{
	moveBVHs(0);
	applyGravity();
	moveObjects();
	computeContinuousBVHs(0);
	detectCollisions();
	respondToCollisions();
}

void PhysicsEngine::computeNetForceTorquePair()
{

}

void PhysicsEngine::applyGravity()
{
	for (auto object : objects)
	{
		object.second.netForce += glm::vec4(0.0, -9.8, 0.0, 0.0);
	}
}

void PhysicsEngine::moveObjects()
{
	float dt = timer.getTime();
	timer.set();
	for (auto object : objects)
	{
		object.second.move(dt);
	}
}

void PhysicsEngine::computeBVH(int handle)
{
	glm::vec3 medianPoint = glm::vec3(0.0, 0.0, 0.0);
	float radius = 0.0;
		for (auto vertex : objects[handle].m_vertices)
		{
			medianPoint += vertex;
			radius += glm::length(vertex);
	    }
		medianPoint /= objects[handle].m_vertices.size();
	objects[handle].bvh.sphere.m_center = medianPoint;
	objects[handle].bvh.sphere.m_radius = radius;
}