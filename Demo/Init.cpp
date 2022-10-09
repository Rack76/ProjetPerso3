#include "Input.h"
#include "Camera.h"
#include "glfw3.h"

void initKeyboardAndMouseInput(GLFWwindow* window, Camera* camera)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glfwSetCursorPos(window, width / 2.0, height / 2.0);
	glfwSetWindowUserPointer(window, static_cast<void*>(camera));
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetKeyCallback(window, keyboardCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
}