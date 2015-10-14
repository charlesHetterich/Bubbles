#pragma once
#include "globalVariablesAndIncludes.h"
#include "AL\al.h"
#include "AL\alc.h"

class Sounds
{
public:

	static void soundsInit()
	{
		device = alcOpenDevice(NULL);
		if (device == NULL)
			std::cout << "cannot open sound card" << std::endl;
		context = alcCreateContext(device, NULL);
		if (context == NULL)
			std::cout << "cannot open context" << std::endl;
		alcMakeContextCurrent(context);
	}
	static void soundsOver()
	{
		alcDestroyContext(context);
		alcCloseDevice(device);
	}
	static ALCdevice* device;
	static ALCcontext* context;

	Sounds(float x, float y, std::string string);
	Sounds(){}
	~Sounds();

	void play();

	void setX(float x) { xPos = x; alSource3f(sourceid, AL_POSITION, xPos / 100, yPos / 100, 0); }
	float getX() { return xPos; alSource3f(sourceid, AL_POSITION, xPos / 100, yPos / 100, 0); }
	void setY(float y) { yPos = y; }
	float getY() { return yPos; }

	void shouldLoop() { alSourcei(sourceid, AL_LOOPING, AL_TRUE); }
	void shouldNotLoop() { alSourcei(sourceid, AL_LOOPING, AL_TRUE); }

protected:

	float xPos, yPos;

	char* data;
	unsigned int sourceid, bufferid;
};

class WorldSounds : public Sounds
{
public:

	static void setListenerPos(float x, float y)
	{
		alListener3f(AL_POSITION, x / 100, y / 100, 0.0f);
	}

	WorldSounds(float x, float y, std::string string) : Sounds(x, y, string)
	{
		alSourcei(sourceid, AL_SOURCE_RELATIVE, AL_FALSE);
	};
	WorldSounds(){}

private:

};