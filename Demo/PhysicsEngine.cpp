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

	float radius;
	glm::vec3 minimumPoint = glm::vec3(minimumX, minimumY, minimumZ);
	glm::vec3 maximumPoint = glm::vec3(maximumX, maximumY, maximumZ);
	if (glm::length(minimumPoint) < glm::length(maximumPoint))
		radius = glm::length(maximumPoint);
	else
		radius = glm::length(minimumPoint);
	glm::vec3 center = minimumPoint + 0.5f * (maximumPoint - minimumPoint);
	bvh->sphere.m_center = center;
	bvh->sphere.m_radius = radius;
	bvh->aabb.bottomLeftCorner = minimumPoint;
	bvh->aabb.topRightCorner = maximumPoint;
	bvh->aabb.center = center;

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