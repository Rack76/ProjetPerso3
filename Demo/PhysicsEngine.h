#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include "FaceInfo.h"
#include "EdgeInfo.h"
#include "PhysicsObject.h"
#include "BVH.h"
#include <map>
#include <queue>
#include <vector>

class PhysicsEngine
{
public:
	PhysicsEngine(int maximumObjectCount)
	{
		for (int i = 0; i < maximumObjectCount; i++)
		{
			handles.push(i);
		}
	}
	 int registerObject(std::vector<FaceInfo> faceInfoList, std::vector<EdgeInfo> edgeInfoList
		, float mass, float cof, float cor
		, glm::vec3 position
		, glm::mat4 orientation, BVH bvh);
	 void mapObject(int handle, glm::vec3& position, glm::mat4& orientation);
	void deleteObject(int handle);
private:
	void computeObjectExtendedRepresentation();
	std::map<int, PhysicsObject> objects;
	std::queue<int> handles;
};

#endif 