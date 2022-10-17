#include "glew.h"
#include "Shader.h"
#include "Render.h"
#include "glfw3.h"
#include "Init.h"
#include "Camera.h"
#include "Object.h"
#include "PhysicsEngine.h"
#include <iostream>

int main(int argc, char** argv)
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(800, 600, "window", NULL, NULL);
	Camera camera;
	PhysicsEngine physics(2);
	initKeyboardAndMouseInput(window, &camera);
	std::vector<RendererObject*> objects;
	objects.push_back(new RendererObject("cube.obj", "brick1.jpg", 0.0, 0.0, 0.0, 1.0, 1.0, 0.5));
	Renderer renderer(objects, &physics);
	Shader shader(&camera, objects[0]);
	

	while (!glfwWindowShouldClose(window))
	{
		getAndProcessInputs(window, &camera);
		camera.update();
		renderer.render(window, &shader, &camera);
		glfwPollEvents();
	}

	objects.clear(); 
}