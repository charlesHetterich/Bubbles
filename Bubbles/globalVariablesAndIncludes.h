#pragma once
#include <SDL\SDL.h>
#include <string>
#include <GL/glew.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <memory>
#include <time.h>
#include <cstdlib>
#include <fstream>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "mathFunctions.h"

static double FRAME_CAP = 240.0;
extern float SCREEN_WIDTH;
extern float SCREEN_HEIGHT;
extern float ACTUAL_SCREEN_WIDTH;
extern float ACTUAL_SCREEN_HEIGHT;

extern float SCREEN_RATIO;

struct Rect
{
	Rect(float x = 0, float y = 0, float w = 0, float h = 0)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}

	float x;
	float y;
	float w;
	float h;
};

static bool circleCollision(float x1, float y1, float r1, float x2, float y2, float r2)
{
	float distance = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));

	if (r1 + r2 >= distance)
		return true;

	return false;
}