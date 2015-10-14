#include "Time.h"
Time::Time()
{
}

Time::~Time()
{
}

long Time::getTime()
{
	return (long)SDL_GetPerformanceCounter();
}

double Time::getDelta()
{
	return delta;
}

void Time::setDelta(double delta)
{
	this->delta = delta;
}

long Time::getClickFrequency()
{
	return (long)SDL_GetPerformanceFrequency();
}