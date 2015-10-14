#pragma once
#include "Display.h"
#include "Time.h"
#include "Game.h"
#include "Input.h"



class MainComponent
{
public:
	MainComponent();
	~MainComponent();

	void start();
	void stop();

private:

	void run();
	void render();
	void cleanUp();

	//game
	Game* game;

	//fps counter
	int frames = 0;
	long frameCounter = 0;

	//core engine
	bool isRunning;
	Input input;
	CDisplay window = CDisplay(800, 800, "Lone Assassin");
	Time timer;
};

