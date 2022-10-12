#include "Shader.h"

void Shader::createShader()
{
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	loadVertexShader();
	loadFragmentShader();
	GLint length = 500;
	const char* temp1 = vertexShaderString.c_str();
	const char* temp2 = fragmentShaderString.c_str();
	glShaderSource(vertexShader, 1, &temp1, NULL);
	glShaderSource(fragmentShader, 1, &temp2, NULL);
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);
	GLint compilationStatus;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compilationStatus);
	if (!compilationStatus)
	{
		logShaderCompilationStatus("vertexShaderLog.txt", vertexShader);
		exit(EXIT_FAILURE);
	}
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compilationStatus);
	if (!compilationStatus)
	{
		logShaderCompilationStatus("fragmentShaderLog.txt", fragmentShader);
		exit(EXIT_FAILURE);
	}
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	GLint linkingStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkingStatus);
	if (!linkingStatus)
	{
		logProgramLinkingStatus();
		exit(EXIT_FAILURE);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::useShaderProgram()
{
	glUseProgram(program);
}


void Shader::loadVertexShader()
{
	std::string string;
	std::ifstream file("vertexShader.txt", std::ios::in);
	while (std::getline(file, string))
	{
		vertexShaderStringOss << string << '\n';
	}
	vertexShaderString = vertexShaderStringOss.str();
}

void Shader::loadFragmentShader()
{
	std::string string;
	std::ifstream file("fragmentShader.txt", std::ios::in);
	while (std::getline(file, string))
	{
		fragmentShaderStringOss << string << '\n';
	}
	fragmentShaderString = fragmentShaderStringOss.str();
}

void Shader::logShaderCompilationStatus(std::string fileName, GLint shader)
{
	std::ofstream file(fileName, std::ios::out);
	std::string string;
	GLchar logStatus[1000];
	glGetShaderInfoLog(shader, 1000, 0, logStatus);
	std::istringstream iss(std::string(logStatus), std::ios::in);
	while (iss)
	{
		iss >> string;
		file << string;
	}
	
}

void Shader::logProgramLinkingStatus()
{
	std::ofstream file("shaderProgramLinkingStatusLog.txt", std::ios::out);
	std::string string;
	GLchar logStatus[1000];
	glGetProgramInfoLog(program, 1000, nullptr, logStatus);
	std::istringstream iss(std::string(logStatus), std::ios::in);
	while (iss)
	{
		iss >> string;
		file << string;
	}
}

void Shader::doShaderPlumbing(Camera* camera, RendererObject* object)
{
	int worldID = glGetUniformLocation(program, "world");
	glUniformMatrix4fv(worldID, 1, GL_FALSE, object->getWorldArray());

	int viewID = glGetUniformLocation(program, "view");
	glUniformMatrix4fv(viewID, 1, GL_FALSE, camera->getViewArray());

	int projectionID = glGetUniformLocation(program, "projection");
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, camera->getProjectionArray());
}