#ifndef BVH_H
#define BVH_H

#include "Sphere.h"
#include "Face.h"
#include <vector>

class BVH
{
public:
	BVH() {

	}
	std::vector<Face> faces;
	Sphere sphere;
	BVH* node0;
	BVH* node1;
};

#endif 