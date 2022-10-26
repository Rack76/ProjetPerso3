#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include "FaceInfo.h"
#include "EdgeInfo.h"
#include "PhysicsObject.h"
#include "BVH.h"
#include "Timer.h"
#include "Plane.h"
#include "Face.h"
#include "CollisionInfo.h"
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
	void computeBVH2(std::vector<glm::vec3> vertices, BVH* bvh, std::vector<Face>& faces);
	float respondToCollisions(float dt);
	void moveBVHs(BVH* bvh0, BVH* bvh1,
		int handle0, int handle1);
	void computeContinuousBVHs(int level);
	void detectCollisions(float dt, float startingTime);
	void bvhIntersect(int handle0, int handle1, float startingTime);
	bool sphereIntersect(BVH* bvh0, BVH* bvh1,
		int handle0, int handle1, float* time, float startingTime);
	bool trianglesIntersect(const std::vector<Face>& faces0, const std::vector<Face>& faces1, float *time, float startingTime);
	void computeObjectExtendedRepresentation(int handle);
	void computeNetForceTorquePair();
	void applyGravity();
	void moveObjects(float dt);
	std::map<int, PhysicsObject> objects;
	std::vector<std::pair<int, int>> listOfPotentialCollisions;
	std::map<float, CollisionInfo> globalCollisionList;
	std::vector<CollisionInfo> collisionGroup;
	std::vector<CollisionInfo> contactList;
	std::vector<int> collidingObjects;
	std::vector<int> objectsInContact;
	std::queue<int> handles;
	Timer timer;
};

#endif 