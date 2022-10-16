#ifndef EDGE_INFO_H
#define EDGE_INFO_H

#include "glm/glm.hpp"
#include "glm/ext.hpp"

class FaceInfo;

class EdgeInfo
{
public:
	EdgeInfo() {
		
	}
	EdgeInfo(glm::vec3 _v0, glm::vec3 _v1) {
		v0 = _v0;
		v1 = _v1;
	}
	glm::vec3 v0;
	glm::vec3 v1;
	FaceInfo* faceInfoLeft;
	FaceInfo* faceInfoRight;
	bool operator==(const EdgeInfo& edgeInfo) const
	{
		if (((this->v0.x == edgeInfo.v0.x &&
			 this->v0.y == edgeInfo.v0.y &&
			 this->v0.z == edgeInfo.v0.z) &&
			(this->v1.x == edgeInfo.v1.x &&
			 this->v1.y == edgeInfo.v1.y &&
			 this->v1.z == edgeInfo.v1.z)) ||
		   ((this->v0.x == edgeInfo.v1.x &&
			 this->v0.y == edgeInfo.v1.y &&
			 this->v0.z == edgeInfo.v1.z) &&
		    (this->v1.x == edgeInfo.v0.x &&
		     this->v1.y == edgeInfo.v0.y &&
		     this->v1.z == edgeInfo.v0.z)))
			return true;
		return false;
	}
};

#endif
