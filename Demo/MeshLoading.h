#ifndef MESHLOADING_H
#define MESHLOADING_H

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include "glew.h"

GLfloat* loadOBJMesh(std::string filename, int& size, GLfloat* textureCoordArray, int& textCoordSize);
GLfloat* createArray(const std::vector<GLfloat>& v);

#endif 
