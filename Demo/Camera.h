#ifndef CAMERA_H
#define CAMERA_H

#define _USE_MATH_DEFINES
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "glfw3.h"

class Camera
{
public:
	Camera();

	float* getViewArray() {
		return &view[0][0];
	}
	float* getProjectionArray() {
		return &projection[0][0];
	}

	void rotate(GLFWwindow* window, float dx, float dy);
	void moveCamera();
	void forward();
	void backward();
	void leftward();
	void rightward();
	void upward();
	void downward();
private:
	void setUpCameraFrame();
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec3 position;
	glm::vec3 orientation;
	glm::vec3 up;
	glm::vec3 right;
	float yaw;
	float pitch;
};

#endif