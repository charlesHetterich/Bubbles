#pragma once
#include "globalVariablesAndIncludes.h"

class CDisplay
{
public:
	CDisplay(int w, int h, const std::string& title);
	~CDisplay();

	void clear(float r, float g, float b, float a);
	void updateWindow();
	bool getClosed();

private:

	SDL_Window* window;
	SDL_GLContext glContext;
	bool isClosed;

};

