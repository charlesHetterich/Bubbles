#pragma once
#include "globalVariablesAndIncludes.h"

const static int NUM_KEYCODES = 256;
const static int NUM_MOUSECODES = 3;
const static float GAMEPAD_AXIS_MAX = 32000.0f;
enum gamePadButtons
{
	GAMEPAD_A = 10,
	GAMEPAD_B = 11,
	GAMEPAD_X = 12,
	GAMEPAD_Y = 13,
	GAMEPAD_HOME = 14,
	GAMEPAD_LB = 8,
	GAMEPAD_RB = 9,
	GAMEPAD_SELECT = 5,
	GAMEPAD_START = 4,
	GAMEPAD_L3 = 6,
	GAMEPAD_R3 = 7,
	GAMEPAD_DPAD_UP = 0,
	GAMEPAD_DPAD_DOWN = 1,
	GAMEPAD_DPAD_LEFT = 2,
	GAMEPAD_DPAD_RIGHT = 3,

	NUM_GAMEPADCODES = 15,

	GAMEPAD_LS_X = 0,
	GAMEPAD_LS_Y = 1,
	GAMEPAD_RS_X = 2,
	GAMEPAD_RS_Y = 3,
	GAMEPAD_LT = 4,
	GAMEPAD_RT = 5,
};
enum gamePadSticks
{
	GAMEPAD_LEFT_STICK,
	GAMEPAD_RIGHT_STICK
};

class Input
{
public:
	Input();
	~Input();

	//gamepad
	SDL_Joystick* gamePad;
	int deadZone;
	bool getGamePadButton(int button);
	bool getGamePadButtonDown(int button);
	bool getGamePadButtonUp(int button);
	int getGamePadAxis(int axis);
	float getGamePadStickAngle(int stick);
	float getGamePadStickTotal(int stick);


	//keyboard
	bool getKey(int keyCode);
	bool getKeyDown(int keyCode);
	bool getKeyUp(int keyCode);

	//mouse
	int getMouseX();
	int getMouseY();
	bool getMouseButton(int button);
	bool getMouseButtonDown(int button);
	bool getMouseButtonUp(int button);

	//update
	void update();
private:

	//gamepad
	bool gamePadButtonIsPressed[NUM_GAMEPADCODES] = {};
	bool gamePadButtonJustDown[NUM_GAMEPADCODES] = {};
	bool gamePadButtonJustUp[NUM_GAMEPADCODES] = {};

	//keyboard
	bool keyIsPressed[NUM_KEYCODES];
	bool keyJustDown[NUM_KEYCODES];
	bool keyJustUp[NUM_KEYCODES];
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	//mouse
	int button;
	int mouseX;
	int mouseY;
	bool mouseButtonIsPressed[NUM_MOUSECODES];
	bool mouseButtonJustDown[NUM_MOUSECODES];
	bool mouseButtonJustUp[NUM_MOUSECODES];
};