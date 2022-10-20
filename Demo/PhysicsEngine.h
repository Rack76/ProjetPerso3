#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include "FaceInfo.h"
#include "EdgeInfo.h"
#include "PhysicsObject.h"
#include "BVH.h"
#include "Timer.h"
#include "Plane.h"
#include "Face.h"
#include <map>
#include <queue>
#include <vector>
#include <iostream>

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
	~PhysicsEngine()
	{
		objects.clear();
	}
	 int registerObject(std::vector<FaceInfo> faceInfoList, std::vector<EdgeInfo> edgeInfoList, std::vector<glm::vec3> normals,
		 std::vector<glm::vec3> vertices
		, float mass, float cof, float cor
		, glm::vec3 position
		, glm::mat4 orientation);
	 void mapObject(int handle, glm::vec3& position, glm::mat4& orientation);
	void deleteObject(int handle);
	void run();
private:
	void computeBVH2(std::vector<glm::vec3> midpoints, BVH* bvh, std::vector<Face> faces);
	void computeBVs(int handle, BVH** bvh, glm::vec3 medianPoint, float radius, std::vector<Face> faces);
	void computeBVs2(int handle, BVH** bvh, glm::vec3 medianPoint, float radius, std::vector<glm::vec3> faces);
	void respondToCollisions();
	void moveBVHs(int level);
	void computeBVH(int handle);
	void computeContinuousBVHs(int level);
	void detectCollisions();
	void computeObjectExtendedRepresentation(int handle);
	void computeNetForceTorquePair();
	void applyGravity();
	void moveObjects();
	std::map<int, PhysicsObject> objects;
	std::queue<int> handles;
	Timer timer;
};

#endif 