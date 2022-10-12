#include "Camera.h"
#include <iostream>

enum { CAMERA_FORWARD };

void getAndProcessInputs(GLFWwindow* window, Camera* camera)
{
	int Dstate = glfwGetKey(window, GLFW_KEY_D);
	static int Dtemp = GLFW_RELEASE;
	if (Dstate == GLFW_PRESS)
		camera->rightward();
	if (Dstate != Dtemp)
	{
		if (Dstate == GLFW_RELEASE)
			camera->stopMoving();
	}
	Dtemp = Dstate;

	int Astate = glfwGetKey(window, GLFW_KEY_A);
	static int Atemp = GLFW_RELEASE;
	if (Astate == GLFW_PRESS)
		camera->leftward();
	if (Astate != Atemp)
	{
		if (Astate == GLFW_RELEASE)
			camera->stopMoving();
	}
	Atemp = Astate;

	int Wstate = glfwGetKey(window, GLFW_KEY_W);
	static int Wtemp = GLFW_RELEASE;
	if (Wstate == GLFW_PRESS)
		camera->forward();
	if (Wstate != Wtemp)
	{
		if (Wstate == GLFW_RELEASE)
			camera->stopMoving();
	}
	Wtemp = Wstate;

	int Sstate = glfwGetKey(window, GLFW_KEY_S);
	static int Stemp = GLFW_RELEASE;
	if (Sstate == GLFW_PRESS)
		camera->backward();
	if (Sstate != Stemp)
	{
		if (Sstate == GLFW_RELEASE)
			camera->stopMoving();
	}
	Stemp = Sstate;

	int spaceState = glfwGetKey(window, GLFW_KEY_SPACE);
	static int spaceTemp = GLFW_RELEASE;
	if (spaceState == GLFW_PRESS)
		camera->upward();
	if (spaceState != spaceTemp)
	{
		if (spaceState == GLFW_RELEASE)
			camera->stopMoving();
	}
	spaceTemp = spaceState;

	int leftShiftState = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
	static int leftShiftTemp = GLFW_RELEASE;
	if (leftShiftState == GLFW_PRESS)
		camera->downward();
	if (leftShiftState != leftShiftTemp)
	{
		if (leftShiftState == GLFW_RELEASE)
			camera->stopMoving();
	}
	leftShiftTemp = leftShiftState;
}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
	camera->rotate(window, xpos, ypos);
}