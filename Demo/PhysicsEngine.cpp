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
		cof, cor, position, orientation, vertices));
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
	std::vector<Face> faces(objects[handle].m_faceInfoList.size());
	for (int i = 0; i < objects[handle].m_faceInfoList.size(); i++)
	{
		faces[i].v[0] = objects[handle].m_faceInfoList[i].v[0];
		faces[i].v[1] = objects[handle].m_faceInfoList[i].v[1];
		faces[i].v[2] = objects[handle].m_faceInfoList[i].v[2];
	}
	computeBVH2(objects[handle].m_vertices, &(objects[handle].bvh), faces);
}

void PhysicsEngine::run()
{
	float startingTime = 0;
	applyGravity();
	float dt = timer.getTime();
	timer.set();
	moveObjects(dt);
	while(listOfPotentialCollisions.size() != 0)
	{
		detectCollisions(dt, startingTime);
		startingTime = respondToCollisions(dt, startingTime);
		if (startingTime > 1)
			break;
	}
}

void PhysicsEngine::computeNetForceTorquePair()
{

}

void PhysicsEngine::applyGravity()
{
	for (auto &object : objects)
	{
		object.second.netForce += glm::vec4(0.0, -9.8, 0.0, 0.0);
	}
}

void PhysicsEngine::moveObjects(float dt)
{
	for (auto &object : objects)
	{
		object.second.move(dt);
	}
	computeWorldHierarchy();
	buildListOfPotentialCollisions();
}



void PhysicsEngine::computeBVH2(std::vector<glm::vec3> vertices, BVH* bvh, std::vector<Face> &faces)
{	
	bvh->node0 = new BVH;
	bvh->node1 = new BVH;
	float minimumX = std::numeric_limits<float>::max();
	float minimumY = std::numeric_limits<float>::max();
	float minimumZ = std::numeric_limits<float>::max();

	float maximumX = std::numeric_limits<float>::min();
	float maximumY = std::numeric_limits<float>::min();
	float maximumZ = std::numeric_limits<float>::min();

	for (int i = 0; i < vertices.size(); i++)
	{
		if (vertices[i].x < minimumX)
			minimumX = vertices[i].x;
		if (vertices[i].z < minimumZ)
			minimumZ = vertices[i].z;
		if (vertices[i].y < minimumY)
			minimumY = vertices[i].y;
		if (vertices[i].x > maximumX)
			maximumX = vertices[i].x;
		if (vertices[i].y > maximumY)
			maximumY = vertices[i].y;
		if (vertices[i].z > maximumZ)
			maximumZ = vertices[i].z;
	}

	float radius = 0.0f;
	glm::vec3 minimumPoint = glm::vec3(minimumX, minimumY, minimumZ);
	glm::vec3 maximumPoint = glm::vec3(maximumX, maximumY, maximumZ);
	if (glm::length(minimumPoint) < glm::length(maximumPoint))
		radius = glm::length(maximumPoint);
	else
		radius = glm::length(minimumPoint);
	glm::vec3 center = minimumPoint + 0.5f * (maximumPoint - minimumPoint);
	bvh->aabb.bottomLeftCorner = minimumPoint;
	bvh->aabb.topRightCorner = maximumPoint;
	bvh->aabb.center = center;

	std::vector<glm::vec3> facesVertices;
	for (auto& face : faces)
	{
		for (int i = 0; i < 3; i++)
		{
			if(std::find(facesVertices.begin(), facesVertices.end(), face.v[i]) == facesVertices.end())
				facesVertices.push_back(face.v[i]);
		}
	}

	glm::vec3 midpoint = glm::vec3(0.0, 0.0, 0.0);
	for (auto& vertex : facesVertices)
	{
		midpoint += vertex;
	}
	midpoint /= facesVertices.size();

	radius = 0.0;
	for (auto& vertex : facesVertices)
	{
		if (radius < glm::length(vertex - midpoint))
			radius = glm::length(vertex - midpoint);
	}

	bvh->sphere.m_center = midpoint;
	bvh->sphere.m_radius = radius;

	glm::vec3 xAxis = glm::vec3(maximumX - minimumX, 0.0, 0.0);
	glm::vec3 yAxis = glm::vec3(0.0, maximumY - minimumY, 0.0);
	glm::vec3 zAxis = glm::vec3(0.0, 0.0, maximumZ - minimumZ);
	glm::vec3 longestAxis;

	if (glm::length(yAxis) > glm::length(xAxis))
		longestAxis = yAxis;
	else
		longestAxis = xAxis;

	if (glm::length(longestAxis) < glm::length(zAxis))
		longestAxis = zAxis;

	Plane plane;
	plane.normal = glm::normalize(longestAxis);
	plane.distance = glm::dot(plane.normal, bvh->aabb.center);

	std::vector<glm::vec3> midpoints;
	std::vector<Face> faces1;
	std::vector<Face> faces2;
	std::vector<glm::vec3> vertices1;
	std::vector<glm::vec3> vertices2;
	for (auto& face : faces)
	{
		midpoints.push_back((face.v[0] + face.v[1] + face.v[2])/3.0f);
	}

	for (int i = 0; i < midpoints.size(); i++)
	{
		if(plane.isBelow(midpoints[i]))
			faces1.push_back(faces[i]);
		else
			faces2.push_back(faces[i]);
	}

	for (int i = 0; i < vertices.size(); i++)
	{
		if (plane.isBelow(vertices[i]))
			vertices1.push_back(vertices[i]);
		else
			vertices2.push_back(vertices[i]);
	}

	maximumPoint -= longestAxis * 0.5f;
	minimumPoint += longestAxis * 0.5f;

	vertices1.push_back(minimumPoint);
	vertices2.push_back(maximumPoint);

	if (faces.size() == 1)
	{
		bvh->faces.push_back(faces[0]);
		bvh->node0 = nullptr;
		bvh->node1 = nullptr;
		return;
	}
	else if (faces1.size() == faces.size() || faces2.size() == faces.size())
	{
		return;
	}

	computeBVH2(vertices1, bvh->node0, faces1);
	computeBVH2(vertices2, bvh->node1, faces2);
	if (faces1.size() == faces.size() || faces2.size() == faces.size())
	{
		for (auto& face : faces)
		{
			bvh->faces.push_back(face);
		}
		bvh->node0 = nullptr;
		bvh->node1 = nullptr;
	}
}

void PhysicsEngine::moveBVHs(BVH* bvh0, BVH* bvh1,
							 int handle0, int handle1)
{
	bvh0->sphere.m_center = glm::vec3(0.0, 0.0, 0.0);
	bvh1->sphere.m_center = glm::vec3(0.0, 0.0, 0.0);
	bvh0->sphere.m_center += objects[handle0].m_position;
	bvh1->sphere.m_center += objects[handle1].m_position;
}

void PhysicsEngine::computeContinuousBVHs(int level)
{

}

void PhysicsEngine::detectCollisions(float dt, float startingTime)
{
	for (int i = 0 ; i < listOfPotentialCollisions.size(); i++)
	{
		bvhIntersect(listOfPotentialCollisions[i].first, listOfPotentialCollisions[i].second, startingTime);
	}
	listOfPotentialCollisions.clear();
	for (auto& pair : globalCollisionList)
	{
		//fill collisionInfos
		glm::vec4 position0 = objects[pair.second.object0].m_position + objects[pair.second.object0].linearVelocity * pair.first;
		glm::vec4 position1 = objects[pair.second.object1].m_position + objects[pair.second.object1].linearVelocity * pair.first;
		// same for orientations
		//figure out remainding collisionInfos members
	}
	for (auto& pair : globalCollisionList)
	{
		if (std::find(collisionGroup.begin(), collisionGroup.end(), pair.second) != collisionGroup.end())
			continue;
		if (collisionGroup.size() == 0)
		{
			collisionGroup.push_back(pair.second);
			continue;
		}
		if (pair.second.time - collisionGroup[0].time < 0.01)
		{
			collisionGroup.push_back(pair.second);
			continue;
		}
		else
		{
			respondToCollisions(dt, startingTime);
			collisionGroup.push_back(pair.second);
		}
	}
	globalCollisionList.clear();
}

void PhysicsEngine::bvhIntersect(int handle0, int handle1, float startingTime)
{
	float time;
	if (sphereIntersect(&objects[handle0].bvh, &objects[handle1].bvh, handle0, handle1, &time, startingTime));
		globalCollisionList.emplace(time, CollisionInfo(handle0, handle1, time));
}

bool PhysicsEngine::sphereIntersect(BVH *bvh0, BVH *bvh1,
									int handle0, int handle1, float *time, float startingTime)
{
	bool condition0, condition1, condition2, condition3;

	moveBVHs(bvh0, bvh1,
		     handle0, handle1);
	glm::vec4 v0 = objects[handle0].linearVelocity;
	glm::vec4 a0 = glm::vec4(bvh0->sphere.m_center, 1.0f);

	glm::vec4 v1 = objects[handle1].linearVelocity;
	glm::vec4 a1 = glm::vec4(bvh1->sphere.m_center, 1.0f);
	float a = glm::dot(v0-v1, v0-v1);
	float b = 2.0f * glm::dot(a0 - a1, v0 - v1);
	float c  = glm::dot(a0 - a1, a0 - a1);
	float delta = b * b - 4 * a * c;
	if (delta > 0)
	{
		float x1 = (-b - sqrt(delta)) / (2 * a);
		float x2 = (-b + sqrt(delta)) / (2 * a);
		if ((x1 < 0 && x1 > 1 - startingTime) && (x2 < 0 && x2 > 1 - startingTime))
			return false;
	}
	else if (delta == 0)
	{
		float x0 = -b / (2 * a);
		if (x0 < 0 && x0 > 1 - startingTime)
			return false;
	}
	else
		return false;

	if (bvh0->node0 == nullptr)
		return trianglesIntersect(bvh0->faces, bvh1->faces, time, startingTime);
	 
	condition0 = sphereIntersect(bvh0->node0, bvh1->node0, handle0, handle1, time, startingTime);
	float time0 = *time;
	condition1 = sphereIntersect(bvh0->node0, bvh1->node1, handle0, handle1, time, startingTime);
	if (*time > time0)
		*time = time0;
	time0 = *time;
	condition2 = sphereIntersect(bvh0->node1, bvh1->node0, handle0, handle1, time, startingTime);
	if (*time > time0)
		*time = time0;
	time0 = *time;
	condition3 = sphereIntersect(bvh0->node1, bvh1->node1, handle0, handle1, time, startingTime);
	if (*time > time0)
		*time = time0;
	return (condition0 || condition1 || condition2 || condition3);
}

bool PhysicsEngine::trianglesIntersect(const std::vector<Face> &faces0, const std::vector<Face> &faces1, float *time, float startingTime)
{

}

float PhysicsEngine::respondToCollisions(float dt, float startingTime)
{
	//set final positions and orientations
	float time = collisionGroup[0].time;
	if (time > 1)
		return time;
	for (auto& collision : collisionGroup)
	{
		if (std::find(collidingObjects.begin(), collidingObjects.end(), collision.object0) == collidingObjects.end())
		{
			collidingObjects.push_back(collision.object0);
		}
		if (std::find(collidingObjects.begin(), collidingObjects.end(), collision.object1) == collidingObjects.end())
		{
			collidingObjects.push_back(collision.object1);
		}
	}
	for (auto& object : collidingObjects)
	{
		objects[object].m_position += objects[object].linearVelocity * (time);
		//same for orientations
	}
	for (auto& collision : collisionGroup)
	{
		//check if the collision is a contact, if so, remove it from collision group add it to the list of contacts
		//compute collision impulses, convert each to a force-torque pair and add them to the objects
	}	
	for (auto& object : collidingObjects)
	{
		objects[object].move(dt - time);
	}
	collidingObjects.clear();
	for (auto& contact : contactList)
	{
		if (std::find(collisionGroup.begin(), collisionGroup.end(), contact) != collisionGroup.end())
		{
			/*check if the contact is still valid, if not, move it from list of contacts to collision group,
			or if contact breaks apart, just removes it*/
		}
	}
	for (auto& object : collidingObjects)
	{
		objects[object].move(dt - time);
	}
	collidingObjects.clear();
	for (auto& contact : contactList)
	{
		if (std::find(objectsInContact.begin(), objectsInContact.end(), contact.object0) == objectsInContact.end())
		{
			objectsInContact.push_back(contact.object0);
		}
		if (std::find(objectsInContact.begin(), objectsInContact.end(), contact.object1) == objectsInContact.end())
		{
			objectsInContact.push_back(contact.object1);
		}
	}
	//compute contact forces, convert each to a force-torque pair and add them to the objects
	for (auto& object : objectsInContact)
	{
		objects[object].move(dt - time);
	}
	collisionGroup.clear();
	contactList.clear();
	objectsInContact.clear();
	return time;
}