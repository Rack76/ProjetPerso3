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
	std::vector<glm::vec3> midpoints;
	for (int i = 0; i < objects[handle].m_faceInfoList.size(); i++)
	{
		glm::vec3 midpoint = glm::vec3(0.0, 0.0, 0.0);
		for (int j = 0; j < 3; j++)
		{
			midpoint += objects[handle].m_faceInfoList[i].v[j];
		}
		midpoint /= 3;
		midpoints.push_back(midpoint);
	}
	std::vector<Face> faces(objects[handle].m_faceInfoList.size());
	for (int i = 0; i < objects[handle].m_faceInfoList.size(); i++)
	{
		faces[i].v[0] = objects[handle].m_faceInfoList[i].v[0];
		faces[i].v[1] = objects[handle].m_faceInfoList[i].v[1];
		faces[i].v[2] = objects[handle].m_faceInfoList[i].v[2];
	}
	computeBVH2(midpoints, &(objects[handle].bvh), faces);
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
	for (auto &object : objects)
	{
		object.second.netForce += glm::vec4(0.0, -9.8, 0.0, 0.0);
	}
}

void PhysicsEngine::moveObjects()
{
	float dt = timer.getTime();
	timer.set();
	for (auto &object : objects)
	{
		object.second.move(dt);
	}
}

void PhysicsEngine::computeBVH2(std::vector<glm::vec3> midpoints, BVH* bvh, std::vector<Face> faces)
{	
	bvh->node0 = new BVH;
	bvh->node1 = new BVH;
	float minimumX = std::numeric_limits<float>::max();
	float minimumY = std::numeric_limits<float>::max();
	float minimumZ = std::numeric_limits<float>::max();

	float maximumX = std::numeric_limits<float>::min();
	float maximumY = std::numeric_limits<float>::min();
	float maximumZ = std::numeric_limits<float>::min();

	for (int i = 0; i < midpoints.size(); i++)
	{
		if (midpoints[i].x < minimumX)
			minimumX = midpoints[i].x;
		if (midpoints[i].z < minimumZ)
			minimumZ = midpoints[i].z;
		if (midpoints[i].y < minimumY)
			minimumY = midpoints[i].y;
		if (midpoints[i].x > maximumX)
			maximumX = midpoints[i].x;
		if (midpoints[i].z > maximumY)
			maximumY = midpoints[i].z;
		if (midpoints[i].y > maximumZ)
			maximumZ = midpoints[i].y;
	}

	glm::vec3 minimumPoint = glm::vec3(minimumX, minimumY, minimumZ);
	glm::vec3 maximumPoint = glm::vec3(maximumX, maximumY, maximumZ);
	glm::vec3 center = minimumPoint + 0.5f * (maximumPoint - minimumPoint);
	float radius = glm::length(center - minimumPoint);
	bvh->sphere.m_center = center;
	bvh->sphere.m_radius = radius;

	glm::vec3 xAxis = glm::vec3(maximumX - minimumX, 0.0, 0.0);
	glm::vec3 yAxis = glm::vec3(0.0, maximumY - minimumY, 0.0);
	glm::vec3 zAxis = glm::vec3(0.0, 0.0, maximumZ - minimumZ);

	float longestAxisLength = std::numeric_limits<float>::min();
	glm::vec3 longestAxis;

	if (glm::length(xAxis) > longestAxisLength)
	{
		longestAxisLength = glm::length(xAxis);
		longestAxis = xAxis;
	}
	if (glm::length(yAxis) > longestAxisLength)
	{
		longestAxisLength = glm::length(yAxis);
		longestAxis = yAxis;
	}
	if (glm::length(zAxis) > longestAxisLength)
	{
		longestAxisLength = glm::length(zAxis);
		longestAxis = zAxis;
	}

	glm::vec3 interectionPoint = minimumPoint + 0.5f * longestAxis;

	Plane plane;
	plane.normal = glm::normalize(longestAxis);
	plane.distance = glm::dot(plane.normal, interectionPoint);

	std::vector<glm::vec3> midpoints1;
	std::vector<glm::vec3> midpoints2;

	std::vector<Face> faces1;
	std::vector<Face> faces2;

	for (int i = 0; i < midpoints.size(); i++)
	{
		if (plane.isBelow(midpoints[i]))
		{
			midpoints1.push_back(midpoints[i]);
			faces1.push_back(faces[i]);
		}
		else
		{
			midpoints2.push_back(midpoints[i]);
			faces2.push_back(faces[i]);
		}
	}

	if (midpoints1.size() == midpoints.size())
	{
		for (int i = 0; i < midpoints1.size() - 1; i++)
		{
			bvh->faces.push_back(Face());
		}
		for (int i = 0; i < midpoints1.size(); i++)
		{
			bvh->faces[i] = faces[i];
		}
		bvh->node0 = nullptr;
		bvh->node1 = nullptr;
	}
	else if (midpoints1.size() > 1)
		computeBVH2(midpoints1, bvh->node0, faces1);
	else if (midpoints1.size() == 1)
	{
		float radius = 0.0;
		for (int i = 0; i < 3; i++)
		{
			if (glm::length(faces1[0].v[i] - midpoints1[0]) > radius)
				radius = glm::length(faces1[0].v[i] - midpoints1[0]);
		}
		bvh->node0->faces[0] = faces1[0];
		bvh->node0->sphere.m_center = midpoints1[0];
		bvh->node0->sphere.m_radius = radius;
	}

	if (midpoints2.size() == midpoints.size())
	{
		for (int i = 0; i < midpoints2.size() - 1; i++)
		{
			bvh->faces.push_back(Face());
		}
		for (int i = 0; i < midpoints2.size(); i++)
		{
			bvh->faces[i] = faces[i];
		}
		bvh->node0 = nullptr;
		bvh->node1 = nullptr;
	}
	else if (midpoints2.size() > 1)
		computeBVH2(midpoints2, bvh->node1, faces2);
	else if(midpoints2.size() == 1)
	{
		float radius = 0.0;
		for (int i = 0; i < 3; i++)
		{
			if (glm::length(faces2[0].v[i] - midpoints2[0]) > radius)
				radius = glm::length(faces2[0].v[i] - midpoints2[0]);
		}
		bvh->node1->faces[0] = faces2[0];
		bvh->node1->sphere.m_center = midpoints2[0];
		bvh->node1->sphere.m_radius = radius;
	}
}

void PhysicsEngine::moveBVHs(int level)
{

}

void PhysicsEngine::computeContinuousBVHs(int level)
{

}

void PhysicsEngine::detectCollisions()
{

}

void PhysicsEngine::respondToCollisions()
{

}