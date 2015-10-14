#include "Texture.h"
#include "stb_image.h"
#include <cassert>
#include <iostream>

CTexture::CTexture(const std::string fileName)
{
	int width, height, numComponents;
	
	std::string file = "./res/images/" + fileName;
	unsigned char* imageData = stbi_load(file.c_str(), &width, &height, &numComponents, 4);

	if (imageData == NULL)
		std::cerr << "Texture loading failed for texture: " << fileName << std::endl;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	stbi_image_free(imageData);

	glBindTexture(GL_TEXTURE_2D, 0);
}

CTexture::CTexture()
{
	
	unsigned char* imageData = new unsigned char[4];
	for (int i = 0; i < 4; i++)imageData[i] = 255;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, 1, 1, 0, GL_DEPTH_COMPONENT24, GL_FLOAT, NULL);
}

CTexture::CTexture(unsigned int width, unsigned int height)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT24, GL_FLOAT, 0);
}

GLuint CTexture::getTextureData()
{
	return texture;
}

void CTexture::bind(unsigned int unit)
{
	assert(unit >= 0 && unit <= 31);

	glActiveTexture(GL_TEXTURE + unit);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void CTexture::unBind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

CTexture::~CTexture()
{
	glDeleteTextures(1, &texture);
}