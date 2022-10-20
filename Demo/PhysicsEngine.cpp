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
	computeBVH(handle);
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

void PhysicsEngine::computeBVH(int handle)
{
	glm::vec3 medianPoint = glm::vec3(0.0, 0.0, 0.0);
	float radius = 0.0;
	for (auto &vertex : objects[handle].m_vertices)
	{
		medianPoint += vertex;
    }
	medianPoint /= objects[handle].m_vertices.size();
	for (auto& vertex : objects[handle].m_vertices)
	{
		if (glm::length(vertex - medianPoint) > radius)
			radius = glm::length(vertex - medianPoint);
	}
	objects[handle].bvh.sphere.m_center = medianPoint;
	objects[handle].bvh.sphere.m_radius = radius;
	std::vector<glm::vec3> vertices(objects[handle].m_vertices.size());
	for (int i = 0; i < objects[handle].m_vertices.size(); i++)
	{
		vertices[i] = objects[handle].m_vertices[i];
	}

	BVH* bvh = &objects[handle].bvh;
	computeBVs2(handle, &bvh, medianPoint, radius, vertices);
}

void PhysicsEngine::computeBVs(int handle, BVH** bvh, glm::vec3 medianPoint, float radius, std::vector<Face> faces)
{
	std::vector<glm::vec3> firstNodeVertices;
	std::vector<glm::vec3> secondNodeVertices;
	Plane plane;
	plane.distance = glm::dot(plane.normal, medianPoint);
	(*bvh)->node0 = new BVH;
	(*bvh)->node1 = new BVH;
	for (auto& face : faces)
	{
		medianPoint = glm::vec3(0.0, 0.0, 0.0);
		for (int i = 0; i < 3; i++)
		{
			medianPoint += face.v[i];
		}
		medianPoint /= 3;
		if (plane.isBelow(medianPoint))
		{
			(*bvh)->node0->faces.push_back(face);
			for (int j = 0; j < 3; j++)
			{
					firstNodeVertices.push_back(face.v[j]);
			}
		}
		else
		{
			(*bvh)->node1->faces.push_back(face);
			for (int j = 0; j < 3; j++)
			{
					secondNodeVertices.push_back(face.v[j]);
			}
		}
	}

	medianPoint = glm::vec3(0.0, 0.0, 0.0);
	radius = 0.0;
	for (auto& vertex : firstNodeVertices)
	{
		medianPoint += vertex;
	}
	medianPoint /= firstNodeVertices.size();
	for (auto& vertex : firstNodeVertices)
	{
		if (glm::length(vertex - medianPoint) > radius)
			radius = glm::length(vertex - medianPoint);
	}
	Sphere sphere1(medianPoint, radius);
	(*bvh)->node0->sphere = sphere1;
	if ((*bvh)->node0->faces.size() > 1)
	{
		std::cout << (*bvh)->node0->faces.size() << '\n';
		BVH* _bvh = (*bvh)->node0;
		computeBVs(handle, &_bvh, medianPoint, radius, (*bvh)->node0->faces);
	}
	else
	{
		(*bvh)->node0 = nullptr;
	}

	medianPoint = glm::vec3(0.0, 0.0, 0.0);
	radius = 0.0;
	for (auto& vertex : secondNodeVertices)
	{
		medianPoint += vertex;
	}
	medianPoint /= secondNodeVertices.size();
	for (auto& vertex : secondNodeVertices)
	{
		if (glm::length(vertex - medianPoint) > radius)
			radius = glm::length(vertex - medianPoint);
	}
	Sphere sphere2(medianPoint, radius);
	(*bvh)->node1->sphere = sphere2;
	if ((*bvh)->node1->faces.size() > 1)
	{
		std::cout << (*bvh)->node1->faces.size() << '\n';
		BVH* _bvh = (*bvh)->node1;
		computeBVs(handle, &_bvh, medianPoint, radius, (*bvh)->node1->faces);
	}
	else
	{
		(*bvh)->node1 = nullptr;
	}
}

void PhysicsEngine::computeBVs2(int handle, BVH** bvh, glm::vec3 medianPoint, float radius, std::vector<glm::vec3> vertices)
{
	std::vector<glm::vec3> firstNodeVertices;
	std::vector<glm::vec3> secondNodeVertices;
	Plane plane;
	plane.distance = glm::dot(plane.normal, medianPoint);
	(*bvh)->node0 = new BVH;
	(*bvh)->node1 = new BVH;
	for (auto& vertex : vertices)
	{
		if (plane.isBelow(vertex))
			firstNodeVertices.push_back(vertex);
		else
			secondNodeVertices.push_back(vertex);
	}

	medianPoint = glm::vec3(0.0, 0.0, 0.0);
	radius = 0.0;
	for (auto& vertex : firstNodeVertices)
	{
		medianPoint += vertex;
	}
	medianPoint /= firstNodeVertices.size();
	for (auto& vertex : firstNodeVertices)
	{
		if (glm::length(vertex - medianPoint) > radius)
			radius = glm::length(vertex - medianPoint);
	}
	Sphere sphere1(medianPoint, radius);
	(*bvh)->node0->sphere = sphere1;
	if ((*bvh)->node0->faces.size() > 1)
	{
		std::cout << (*bvh)->node0->faces.size() << '\n';
		BVH* _bvh = (*bvh)->node0;
		computeBVs(handle, &_bvh, medianPoint, radius, (*bvh)->node0->faces);
	}
	else
	{
		(*bvh)->node0 = nullptr;
	}

	medianPoint = glm::vec3(0.0, 0.0, 0.0);
	radius = 0.0;
	for (auto& vertex : secondNodeVertices)
	{
		medianPoint += vertex;
	}
	medianPoint /= secondNodeVertices.size();
	for (auto& vertex : secondNodeVertices)
	{
		if (glm::length(vertex - medianPoint) > radius)
			radius = glm::length(vertex - medianPoint);
	}
	Sphere sphere2(medianPoint, radius);
	(*bvh)->node1->sphere = sphere2;
	if ((*bvh)->node1->faces.size() > 1)
	{
		std::cout << (*bvh)->node1->faces.size() << '\n';
		BVH* _bvh = (*bvh)->node1;
		computeBVs(handle, &_bvh, medianPoint, radius, (*bvh)->node1->faces);
	}
	else
	{
		(*bvh)->node1 = nullptr;
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