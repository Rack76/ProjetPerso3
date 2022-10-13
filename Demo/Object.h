#ifndef OBJECT_H
#define OBJECT_H

#include "glew.h"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/ext.hpp"
#include "MeshLoading.h"
#include <vector>
#include <iostream>

class RendererObject
{
public:

	RendererObject(std::string meshFilename, std::string imageFilename, float x, float y, float z);
	~RendererObject();
	void setOrientation(float angle, float x, float y, float z);
	void setPosition(float x, float y, float z);
	void update();
	void bind();
	float* getWorldArray() {
		return &world[0][0];
	}
	int vertexPositionsSize;
	GLuint vbo;
	GLuint textureCoordinatesName;
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
	glm::vec4 position;
	glm::mat4 world;
	glm::mat4 orientation;
	std::vector<std::vector<glm::vec3>> triangles;
	std::vector<std::vector<glm::vec3>> edges;
	std::vector<glm::vec3>

};

#endif