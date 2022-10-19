#ifndef TIMER_H
#define TIMER_H

#include <glfw3.h>

class Timer
{
public:
	void set();
	float getTime();
private:
	float time = 0.0;
};

#endif