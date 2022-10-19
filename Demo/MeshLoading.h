#ifndef MESHLOADING_H
#define MESHLOADING_H

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "EdgeInfo.h"
#include "FaceInfo.h"
#include "glew.h"

GLfloat* loadOBJMesh(std::string filename, 
	int& size, 
	GLfloat** textureCoordArray, 
	int& textCoordSize, 
	std::vector<GLfloat>& vertexPositions, 
	std::vector<GLfloat> &unsortedVertexArray, 
	std::vector<int> &order);
void meshInfo(const int faceCount, std::vector<FaceInfo>& faceInfoList, std::vector<EdgeInfo>& edgeInfoList, std::vector<glm::vec3> &normals, const std::vector<GLfloat>& sortedVertexArray, const std::vector<GLfloat>& unsortedVertexArray, const std::vector<int>& order);

GLfloat* createArray(const std::vector<GLfloat>& v);

#endif 
