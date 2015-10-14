#pragma once
#include "globalVariablesAndIncludes.h"

class Time
{
public:
	Time();
	~Time();

	long getTime();
	double getDelta();
	void setDelta(double delta);
	long getClickFrequency();

private:

	double delta;
};

