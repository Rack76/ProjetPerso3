#ifndef INPUT_H
#define INPUT_H

#include "glfw3.h"
#include "Camera.h"

void cursorPosCallback(GLFWwindow * window, double xpos, double ypos);
void getAndProcessInputs(GLFWwindow* window, Camera* camera);

#endif