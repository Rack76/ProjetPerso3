#ifndef SHADER_H
#define SHADER_H

#include "glew.h"
#include <fstream>
#include <sstream>
#include <string>
#include "Camera.h"

class Shader
{
public:
	Shader(Camera* camera)
	{
		createShader();
		useShaderProgram();
		doShaderPlumbing(camera);
	}
	void doShaderPlumbing(Camera* camera);

private:
	void useShaderProgram();
	void createShader();
	void loadVertexShader();
	void loadFragmentShader();
	void logShaderCompilationStatus(std::string fileName, GLint shader);
	void logProgramLinkingStatus();

	GLint program;
	GLint vertexShader;
	GLint fragmentShader;
	std::string vertexShaderString;
	std::string fragmentShaderString;
	std::ostringstream vertexShaderStringOss;
	std::ostringstream fragmentShaderStringOss;
};

#endif