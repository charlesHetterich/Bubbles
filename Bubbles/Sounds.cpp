#include "Sounds.h"

bool isBigEndian()
{
	int a = 1;
	return !((char*)&a)[0];
}
int convertToInt(char* buffer, int len)
{
	int a = 0;

	if (!isBigEndian())
		for (int i = 0; i < len; i++)
			((char*)&a)[i] = buffer[i];
	else
		for (int i = 0; i < len; i++)
			((char*)&a)[3 - i] = buffer[i];

	return a;
}
char* loadWAV(const char* fn, int& chan, int& samplerate, int& bps, int& size)
{
	char buffer[4];
	std::ifstream in(fn, std::ios::binary);
	in.read(buffer, 4);
	if (strncmp(buffer, "RIFF", 4) != 0)
	{
		std::cout << fn << " is not a .wav file!" << std::endl;
		return NULL;
	}

	in.read(buffer, 4);
	in.read(buffer, 4);	//WAVE
	in.read(buffer, 4);	//fmt
	in.read(buffer, 4);	//16
	in.read(buffer, 2);	//1
	in.read(buffer, 2);
	chan = convertToInt(buffer, 2);
	in.read(buffer, 4);
	samplerate = convertToInt(buffer, 4);
	in.read(buffer, 4);
	in.read(buffer, 2);
	in.read(buffer, 2);
	bps = convertToInt(buffer, 2);
	in.read(buffer, 4); //data
	in.read(buffer, 4);
	size = convertToInt(buffer, 4);
	char* data = new char[size];
	in.read(data, size);
	return data;
}

Sounds::Sounds(float x, float y, std::string string) : xPos(x), yPos(y)
{
	int channel, sampleRate, bps, size;
	data = loadWAV(string.c_str(), channel, sampleRate, bps, size);

	unsigned int format;
	alGenBuffers(1, &bufferid);

	if (channel == 1)
		if (bps == 8)
			format = AL_FORMAT_MONO8;
		else
			format = AL_FORMAT_MONO16;
	else
		if (bps == 8)
			format = AL_FORMAT_STEREO8;
		else
			format = AL_FORMAT_STEREO16;

	alBufferData(bufferid, format, data, size, sampleRate);
	delete[] data;

	alGenSources(1, &sourceid);
	alSourcei(sourceid, AL_BUFFER, bufferid);
	alSourcei(sourceid, AL_SOURCE_RELATIVE, AL_TRUE);
	alSource3f(sourceid, AL_POSITION, xPos / 100, yPos / 100, 0);
}

Sounds::~Sounds()
{
	alDeleteSources(1, &sourceid);
	alDeleteBuffers(1, &bufferid);
}

void Sounds::play()
{
	alSourcePlay(sourceid);
}