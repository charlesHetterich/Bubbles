#include "MainComponent.h"

MainComponent::MainComponent()
{
	isRunning = false;
	game = new Game();
}

MainComponent::~MainComponent()
{
}

void MainComponent::start()
{
	if (isRunning)
		return;

	run();
}

void MainComponent::stop()
{
	if (!isRunning)
		return;

	isRunning = false;
}

void MainComponent::run()
{
	isRunning = true;

	double frameTime = 1.0 / FRAME_CAP;

	long lastTime = timer.getTime();
	double unprocessedTime = 0;

	while (isRunning)
	{
		bool shouldRender = false;

		long startTime = timer.getTime();
		long passedTime = startTime - lastTime;
		lastTime = startTime;

		unprocessedTime += passedTime / (double)timer.getClickFrequency();
		frameCounter += passedTime;

		while (unprocessedTime > frameTime)
		{
			shouldRender = true;

			unprocessedTime -= frameTime;

			if (window.getClosed() || input.getKey(SDL_SCANCODE_ESCAPE))
				stop();

			timer.setDelta(frameTime);
			input.update();

			game->input(input);
			game->update();

			if (frameCounter >= timer.getClickFrequency())
			{
				std::cout << frames << std::endl;
				frames = 0;
				frameCounter = 0;
			}
		}

		if (shouldRender)
		{
			render();
			frames++;
		}
	}

	cleanUp();
}

void MainComponent::render()
{
	window.clear(0.0f / 255.0f, 16.0f / 255.0f, 65.0f / 255.0f, 0.0f);

	game->render();

	window.updateWindow();
}

void MainComponent::cleanUp()
{

}