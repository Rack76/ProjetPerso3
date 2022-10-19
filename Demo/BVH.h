#ifndef BVH_H
#define BVH_H

#include "Sphere.h"

class BVH
{
public:
	BVH() {

	}
	Sphere sphere;
	BVH* node0;
	BVH* node1;
};

#endif 