#include "PhysicsEngine.h"

int PhysicsEngine::registerObject(std::vector<FaceInfo> faceInfoList, std::vector<EdgeInfo> edgeInfoList, std::vector<glm::vec3> normals
	                              ,float mass, float cof, float cor
								  ,glm::vec3 position
								  ,glm::mat4 orientation)
{
	int handle = handles.front();
	handles.pop();
	objects.emplace(handle, PhysicsObject(faceInfoList, edgeInfoList, normals, mass,
		cof, cor, position, orientation)).first;
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
	objects[handle].localFrame =  glm::translate(objects[handle].localInertiaTensor, glm::vec3(objects[handle].centerOfMass));
}

