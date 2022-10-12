#include "Input.h"
#include "Camera.h"
#include "glew.h"
#include "glfw3.h"

void initKeyboardAndMouseInput(GLFWwindow* window, Camera* camera)
{
	int width, height;
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glActiveTexture(GL_TEXTURE0);
	glfwGetWindowSize(window, &width, &height);
	glfwSetCursorPos(window, width / 2.0, height / 2.0);
	glfwSetWindowUserPointer(window, static_cast<void*>(camera));
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetCursorPosCallback(window, cursorPosCallback);
}