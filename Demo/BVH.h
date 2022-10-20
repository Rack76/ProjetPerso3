#ifndef BVH_H
#define BVH_H

#include "Sphere.h"
#include "Face.h"
#include <vector>

class BVH
{
public:
	BVH() {
		faces.push_back(Face());
		node0 = nullptr;
		node1 = nullptr;
	}
	~BVH() {
		if (node0)
			delete node0;
		if (node1)
			delete node1;
	}
	std::vector<Face> faces;
	Sphere sphere;
	BVH* node0;
	BVH* node1;
};

#endif 