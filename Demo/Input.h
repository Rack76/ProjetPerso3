#ifndef INPUT_H
#define INPUT_H

#include "glfw3.h"

void keyboardCallback(GLFWwindow* window, int key, int scanCode, int action, int mods);
void cursorPosCallback(GLFWwindow * window, double xpos, double ypos);

#endif