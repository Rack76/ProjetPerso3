#include "Camera.h"
#include <iostream>
#include "Timer.h"

enum { CAMERA_FORWARD };

void getAndProcessInputs(GLFWwindow* window, Camera* camera)
{
	static Timer timer;
	float dt = timer.getTime();
	timer.set();
	int Dstate = glfwGetKey(window, GLFW_KEY_D);
	static int Dtemp = GLFW_RELEASE;
	if (Dstate == GLFW_PRESS)
		camera->rightward(dt);
	if (Dstate != Dtemp)
	{
		if (Dstate == GLFW_RELEASE)
			camera->stopMoving();
	}
	Dtemp = Dstate;

	int Astate = glfwGetKey(window, GLFW_KEY_A);
	static int Atemp = GLFW_RELEASE;
	if (Astate == GLFW_PRESS)
		camera->leftward(dt);
	if (Astate != Atemp)
	{
		if (Astate == GLFW_RELEASE)
			camera->stopMoving();
	}
	Atemp = Astate;

	int Wstate = glfwGetKey(window, GLFW_KEY_W);
	static int Wtemp = GLFW_RELEASE;
	if (Wstate == GLFW_PRESS)
		camera->forward(dt);
	if (Wstate != Wtemp)
	{
		if (Wstate == GLFW_RELEASE)
			camera->stopMoving();
	}
	Wtemp = Wstate;

	int Sstate = glfwGetKey(window, GLFW_KEY_S);
	static int Stemp = GLFW_RELEASE;
	if (Sstate == GLFW_PRESS)
		camera->backward(dt);
	if (Sstate != Stemp)
	{
		if (Sstate == GLFW_RELEASE)
			camera->stopMoving();
	}
	Stemp = Sstate;

	int spaceState = glfwGetKey(window, GLFW_KEY_SPACE);
	static int spaceTemp = GLFW_RELEASE;
	if (spaceState == GLFW_PRESS)
		camera->upward(dt);
	if (spaceState != spaceTemp)
	{
		if (spaceState == GLFW_RELEASE)
			camera->stopMoving();
	}
	spaceTemp = spaceState;

	int leftShiftState = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
	static int leftShiftTemp = GLFW_RELEASE;
	if (leftShiftState == GLFW_PRESS)
		camera->downward(dt);
	if (leftShiftState != leftShiftTemp)
	{
		if (leftShiftState == GLFW_RELEASE)
			camera->stopMoving();
	}
	leftShiftTemp = leftShiftState;
	camera->setUpCameraFrame();
}

void keyCallback(GLFWwindow* window, int a , int b , int c , int d)
{
	Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
	getAndProcessInputs(window, camera);
}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
	camera->rotate(window, xpos, ypos);
}