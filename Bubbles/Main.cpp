#include "MainComponent.h"

float SCREEN_WIDTH;
float SCREEN_HEIGHT;

float ACTUAL_SCREEN_WIDTH;
float ACTUAL_SCREEN_HEIGHT;

float SCREEN_RATIO;

int main(int argc, char *argv[])
{
	srand((unsigned int)time(NULL));

	MainComponent mainComponent = MainComponent();
	
	mainComponent.start();

	return 0;
}