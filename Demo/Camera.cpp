#include "Camera.h"
#include <iostream>

Camera::Camera()
{
	projection = glm::perspective(20.0 * M_PI / 180.0, 1.2, 1.0, 100.0);
	position = glm::vec3(0.0, 0.0, -1.0);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	yaw = 0;
	pitch = 0;
	setUpCameraFrame();
}

void Camera::rotate(GLFWwindow* window, float dx, float dy)
{
	if ((dx != 0) || (dy != 0))
	{
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		yaw -= (dx - (width / 2.0)) / 100;
		pitch -= (dy - (height / 2.0)) / 100;
		if (pitch > 88.0 * M_PI / 180.0)
			pitch = 88.0 * M_PI / 180.0;
		if (pitch < -88.0 * M_PI / 180.0)
			pitch = -88.0 * M_PI / 180.0;
		setUpCameraFrame();

		glfwSetCursorPos(window, width/2.0, height/2.0);
	}
}

void Camera::moveCamera()
{

}

void Camera::forward()
{
	position += orientation * 0.1f;
	setUpCameraFrame();
}

void Camera::backward()
{
	position = position - orientation * 0.1f;
	setUpCameraFrame();
}

void Camera::leftward()
{
	position -= right * 0.1f;
	setUpCameraFrame();
}

void Camera::rightward()
{
	position += right * 0.1f;
	setUpCameraFrame();
}

void Camera::upward()
{
	position -= up * 0.1f;
	setUpCameraFrame();
}

void Camera::downward()
{
	position += up * 0.1f;
	setUpCameraFrame();
}

void Camera::setUpCameraFrame()
{
		glm::vec3 cameraZ = glm::vec3(cos(pitch) * sin(yaw), sin(pitch), cos(pitch) * cos(yaw)); //cartesian cordinates from yaw and pitch
		orientation = glm::vec3(sin(yaw), 0, cos(yaw));
		glm::vec3 cameraX = glm::normalize(glm::cross(cameraZ, up));
		right = cameraX;
		glm::vec3 cameraY = glm::normalize(glm::cross(cameraX, cameraZ));
		glm::mat4 translationMat = glm::translate(glm::mat4(1.0), -position);
		glm::mat4 translationMat1 = glm::mat4(glm::vec4(1.0, 0.0, 0.0, 0.0),
			                                  glm::vec4(0.0, 1.0, 0.0, 0.0),
											  glm::vec4(0.0, 0.0, 1.0, 0.0),
			                                  glm::vec4(-position.x, -position.y, -position.z, 1.0));
		view = glm::mat4(glm::vec4(cameraX, 0.0f), glm::vec4(cameraY, 0.0f), glm::vec4(-cameraZ, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));	
		view = glm::transpose(translationMat1) * view;
}