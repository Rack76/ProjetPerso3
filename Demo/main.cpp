#include "Shader.h"
#include "Render.h"
#include "glfw3.h"
#include "Init.h"
#include "Camera.h"
#include <iostream>

int main(int argc, char** argv)
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(800, 600, "window", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		std::cout << "f";
	}
	setUpRenderingPipelineData();

	Camera camera;
	Shader shader1(&camera);
	initKeyboardAndMouseInput(window, &camera);

	while (!glfwWindowShouldClose(window))
	{
		shader1.doShaderPlumbing(&camera);
		render(window);
		glfwPollEvents();
	}
}