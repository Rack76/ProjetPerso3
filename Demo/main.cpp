#include "glew.h"
#include "Shader.h"
#include "Render.h"
#include "glfw3.h"
#include "Init.h"
#include "Camera.h"
#include "Object.h"
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

	Camera camera;
	std::vector<RendererObject*> objects;
	objects.push_back(new RendererObject("cube.obj", "brick1.jpg", -2.0, 0.0, 1.0));
	Renderer renderer(objects);
	Shader shader(&camera, objects[0]);
	initKeyboardAndMouseInput(window, &camera);

	while (!glfwWindowShouldClose(window))
	{
		getAndProcessInputs(window, &camera);
		camera.update();
		renderer.render(window, &shader, &camera);
		glfwPollEvents();
	}
}