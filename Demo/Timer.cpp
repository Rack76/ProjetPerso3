#include "Timer.h"

void Timer::set()
{
	time = glfwGetTime();
}

float::Timer::getTime()
{
	return glfwGetTime() - time;
}