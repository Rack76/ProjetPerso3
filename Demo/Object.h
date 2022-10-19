#ifndef OBJECT_H
#define OBJECT_H

#include "glew.h"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/ext.hpp"
#include "MeshLoading.h"
#include "EdgeInfo.h"
#include "FaceInfo.h"
#include <vector>
#include <iostream>

class RendererObject
{
public:

	RendererObject(std::string meshFilename, std::string imageFilename, float x, float y, float z,
					float mass, float cof, float cor);
	~RendererObject();
	void update();
	void bind();
	float* getWorldArray() {
		return &world[0][0];
	}
	int vertexPositionsSize;
	GLuint vbo;
	GLuint textureCoordinatesName;
	int physicsObjectHandle;
	std::vector<EdgeInfo> edgeInfoList;
	std::vector<FaceInfo> faceInfoList;
	std::vector<glm::vec3> normals;
	glm::vec3 position;
	glm::mat4 orientation;
	float m_mass;
	float m_cor;
	float m_cof;
	int physicalStatus; // 0 for a rigid body, 1 for a static object, 2 for an articulated object, 3 for a weapon
	std::vector<glm::vec3> vertices;
private:
	void setUpRenderingPipelineData();
	GLuint vao;
	GLuint texture;
	GLuint textureUnit;
	int textCoordSize;
	int imageWidth;
	int imageHeight;
	int bpp;
	unsigned char* imageData;
	GLfloat* vertexPositions;
	GLfloat** textCoord;
	glm::mat4 world;
	std::vector<GLfloat> vertexPositionsVector;
	std::vector<GLfloat> unsortedVertexArray;
	std::vector<int> order;
};

#endif