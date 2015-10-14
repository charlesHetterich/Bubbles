#include "Display.h"

CDisplay::CDisplay(int w, int h, const std::string& title)
{
	isClosed = false;

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
	glContext = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	GLenum status = glewInit();

	if (status != GLEW_OK)
	{
		std::cerr << "Glew failed to initialize" << std::endl;
		isClosed = true;
	}

	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_STENCIL_TEST);
	//glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask(GL_FALSE);
	glStencilFunc(GL_ALWAYS, 0, 0x00);
	glStencilMask(0xff);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	int W;
	int H;
	SDL_GetWindowSize(window, &W, &H);
	std::cout << "width: " << W << std::endl;
	std::cout << "height: " << H << std::endl;

	SCREEN_RATIO = (float)W / (float)H;
	float idealRatio = 1920.0f / 1080.0f;
	SCREEN_WIDTH = (float)1920;
	SCREEN_HEIGHT = (float)1080;// *((float)H / (float)1080);

	if (SCREEN_RATIO > idealRatio)
		glViewport((W - (float)1920 * ((float)H / 1080.0f)) / 2.0f, 0, (float)1920 * ((float)H / 1080.0f), H);
	else if (SCREEN_RATIO < idealRatio)
		glViewport(0, (H - (float)1080 * ((float)W / 1920.0f)) / 2.0f, W, (float)1080.0f * ((float)W / 1920));

	ACTUAL_SCREEN_WIDTH = W;
	ACTUAL_SCREEN_HEIGHT = H;
}
CDisplay::~CDisplay()
{
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void CDisplay::clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void CDisplay::updateWindow()
{
	SDL_GL_SwapWindow(window);

	SDL_Event e;

	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
			isClosed = true;
	}
}

bool CDisplay::getClosed()
{
	return isClosed;
}