#pragma once

#include <string>
#include <GL/glew.h>
#include <iostream>

class CTexture
{
public:
	CTexture(const std::string fileName);
	CTexture();
	CTexture(unsigned int width, unsigned int height);

	GLuint getTextureData();

	~CTexture();

	void bind(unsigned int unit);
	void unBind();

protected:

	GLuint texture;

};

class RenderTexture : public CTexture
{
public:

	RenderTexture(unsigned int width, unsigned int height)
	{

		GLuint rbo = 0;

		//frame buffer object
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		//texture
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);

		//attach texture to current fbo
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

		//depth buffer object
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

		//DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		//glDrawBuffers(1, DrawBuffers);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "failed to properly create render texture!" << std::endl;

		this->width = width;
		this->height = height;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	RenderTexture(){}

	void fbo_bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glViewport(0, 0, width, height);

		glClearColor(0.06f, 0.06f, 0.06f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // We're not using stencil buffer now
	}
	void fbo_unBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);
	}

private:

	unsigned int width, height;

	GLuint fbo = 0;
};