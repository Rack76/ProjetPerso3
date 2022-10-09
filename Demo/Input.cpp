#include "glfw3.h"
#include "Camera.h"
#include <iostream>

enum { CAMERA_FORWARD };

void keyboardCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
	int cameraDirection = 0;
	if (key == GLFW_KEY_W)
		camera->forward();
	else if (key == GLFW_KEY_S)
		camera->backward();
	else if (key == GLFW_KEY_A)
		camera->leftward();
	else if (key == GLFW_KEY_D)
		camera->rightward();
}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
	camera->rotate(window, xpos, ypos);
}