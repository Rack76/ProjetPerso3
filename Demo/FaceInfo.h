#ifndef FACE_INFO_H
#define FACE_INFO_H

#include "glm/glm.hpp"
#include "glm/ext.hpp"

class EdgeInfo;

class FaceInfo
{
public:
	glm::vec3 v[3];
	EdgeInfo* edgeInfo[3];
};

#endif