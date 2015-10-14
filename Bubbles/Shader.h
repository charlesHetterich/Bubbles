#pragma once
#include "globalVariablesAndIncludes.h"

static std::string loadShader(const std::string& fileName);
static GLuint createShader(const std::string& text, GLenum shaderType);
static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);

static std::string loadShader(const std::string& fileName)
{
	std::ifstream file;
	file.open(fileName.c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}

	return output;
}
static GLuint createShader(const std::string& text, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	if (shader == 0)
		std::cerr << "Error: Shader Creation failed!" << std::endl;

	const GLchar* shaderSourceStrings[1];
	GLint shaderSourceStringLengths[1];

	shaderSourceStrings[0] = text.c_str();
	shaderSourceStringLengths[0] = text.length();

	glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLengths);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error: Program compilation failed: ");

	return shader;
}
static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}

class Shader
{
public:

	static unsigned int CURRENT_SHADER;

	static float BLUR_STRENGTH;

	static enum
	{
		BASIC,
		BLUR_H,
		BLUR_V,
		GRID,
		OUTLINED,
		STENCIL,
		WAVEY,

		NUM_SHADERS
	};
	Shader(std::string filePath);
	~Shader();
	void bind();
	virtual void update(glm::mat4 transform, glm::vec4 color){}
	virtual void update(glm::mat4 transform){}
	virtual void update(glm::mat4 transform, glm::vec2 scale, glm::vec4 color){}
	virtual void updateClock(){}
	GLuint getProgram();

protected:

	void createAttributes()
	{
		glBindAttribLocation(shaderProgram, 0, "position");
		glBindAttribLocation(shaderProgram, 1, "rgbColor");
		glBindAttribLocation(shaderProgram, 2, "texCoords");
	}
	void linkProgram()
	{
		glLinkProgram(shaderProgram);
		CheckShaderError(shaderProgram, GL_LINK_STATUS, true, "Error: Program linking failed: ");
		glValidateProgram(shaderProgram);
		CheckShaderError(shaderProgram, GL_VALIDATE_STATUS, true, "Error: Program is invalid: ");
	}
	void createUniforms(GLuint uniforms[], std::string names[], int numUniforms)
	{
		for (int i = 0; i < numUniforms; i++)
		{
			uniforms[i] = glGetUniformLocation(shaderProgram, names[i].c_str());
		}
	}

	GLuint vertexShader;
	GLuint fragmentShader;

	GLuint shaderProgram;

	//identifier
	unsigned int shaderNumber;
};

class BasicShader : public Shader
{
public:
	BasicShader() : Shader("basic")
	{
		createAttributes();
		linkProgram();

		std::string UniformNames[] = { "transform", "overrideColor" };
		createUniforms(uniforms, UniformNames, NUM_UNIFORMS);

		glUniform3f(uniforms[OVERRIDE_COLOR], 1.0f, 1.0f, 1.0f);

		shaderNumber = BASIC;
	}
	void update(glm::mat4 transform, glm::vec4 color)
	{
		glUniform4f(uniforms[OVERRIDE_COLOR], color.r, color.g, color.b, color.a);
		glUniformMatrix4fv(uniforms[TRANSFORM], 1, GL_FALSE, glm::value_ptr(transform));
	}

private:

	enum
	{
		TRANSFORM,
		OVERRIDE_COLOR,

		NUM_UNIFORMS
	};
	GLuint uniforms[NUM_UNIFORMS];
};

class Blur_H_Shader : public Shader
{
public:
	Blur_H_Shader() : Shader("blurH")
	{
		createAttributes();
		linkProgram();

		std::string UniformNames[] = { "transform", "overrideColor", "blurLevel" };
		createUniforms(uniforms, UniformNames, NUM_UNIFORMS);

		glUniform3f(uniforms[OVERRIDE_COLOR], 1.0f, 1.0f, 1.0f);
		glUniform1f(uniforms[BLUR_LEVEL], 0.0f);

		shaderNumber = BLUR_H;
	}
	void update(glm::mat4 transform, glm::vec4 color)
	{
		glUniform4f(uniforms[OVERRIDE_COLOR], color.r, color.g, color.b, color.a);
		glUniformMatrix4fv(uniforms[TRANSFORM], 1, GL_FALSE, glm::value_ptr(transform));
		glUniform1f(uniforms[BLUR_LEVEL], BLUR_STRENGTH);
	}

private:

	enum
	{
		TRANSFORM,
		OVERRIDE_COLOR,
		BLUR_LEVEL,

		NUM_UNIFORMS
	};
	GLuint uniforms[NUM_UNIFORMS];
};

class Blur_V_Shader : public Shader
{
public:
	Blur_V_Shader() : Shader("blurV")
	{
		createAttributes();
		linkProgram();

		std::string UniformNames[] = { "transform", "overrideColor", "blurLevel" };
		createUniforms(uniforms, UniformNames, NUM_UNIFORMS);

		glUniform3f(uniforms[OVERRIDE_COLOR], 1.0f, 1.0f, 1.0f);
		glUniform1f(uniforms[BLUR_LEVEL], 0.0f);

		shaderNumber = BLUR_V;
	}
	void update(glm::mat4 transform, glm::vec4 color)
	{
		glUniform4f(uniforms[OVERRIDE_COLOR], color.r, color.g, color.b, color.a);
		glUniformMatrix4fv(uniforms[TRANSFORM], 1, GL_FALSE, glm::value_ptr(transform));
		glUniform1f(uniforms[BLUR_LEVEL], BLUR_STRENGTH);
	}

private:

	enum
	{
		TRANSFORM,
		OVERRIDE_COLOR,
		BLUR_LEVEL,

		NUM_UNIFORMS
	};
	GLuint uniforms[NUM_UNIFORMS];
};

class GridShader : public Shader
{
public:
	GridShader() : Shader("grid")
	{
		createAttributes();
		linkProgram();

		std::string UniformNames[] = { "transform", "overrideColor" };
		createUniforms(uniforms, UniformNames, NUM_UNIFORMS);

		glUniform3f(uniforms[OVERRIDE_COLOR], 1.0f, 1.0f, 1.0f);

		shaderNumber = GRID;
	}

	void update(glm::mat4 transform, glm::vec4 color)
	{
		glUniform4f(uniforms[OVERRIDE_COLOR], color.r, color.g, color.b, color.a);
		glUniformMatrix4fv(uniforms[TRANSFORM], 1, GL_FALSE, glm::value_ptr(transform));
	}
private:

	enum
	{
		TRANSFORM,
		OVERRIDE_COLOR,

		NUM_UNIFORMS
	};
	GLuint uniforms[NUM_UNIFORMS];
};

class OutlinedShader : public Shader
{
public:
	OutlinedShader() : Shader("outlined")
	{
		shaderNumber = OUTLINED;
	}

private:
};

class StencilShader : public Shader
{
public:
	StencilShader() : Shader("stencil")
	{
		createAttributes();
		linkProgram();

		std::string UniformNames[] = { "transform" };
		createUniforms(uniforms, UniformNames, NUM_UNIFORMS);

		shaderNumber = STENCIL;
	}

	void update(glm::mat4 transform)
	{
		glUniformMatrix4fv(uniforms[TRANSFORM], 1, GL_FALSE, glm::value_ptr(transform));
	}

private:
	enum
	{
		TRANSFORM,

		NUM_UNIFORMS
	};
	GLuint uniforms[NUM_UNIFORMS];
};

class WaveyShader : public Shader
{
public:
	WaveyShader() : Shader("wavey")
	{
		createAttributes();
		linkProgram();

		std::string UniformNames[] = { "transform", "overrideColor", "scale", "clock" };
		createUniforms(uniforms, UniformNames, NUM_UNIFORMS);

		shaderNumber = WAVEY;
	}

	void update(glm::mat4 transform, glm::vec2 scale, glm::vec4 color)
	{
		glUniformMatrix4fv(uniforms[TRANSFORM], 1, GL_FALSE, glm::value_ptr(transform));
		glUniform4f(uniforms[OVERRIDE_COLOR], color.r, color.g, color.b, color.a);
		glUniform2f(uniforms[SCALE], scale.x, scale.y);
		glUniform1i(uniforms[CLOCK], clock());
	}
	void updateClock()
	{

	}

private:
	enum
	{
		TRANSFORM,
		OVERRIDE_COLOR,
		SCALE,
		CLOCK,

		NUM_UNIFORMS
	};
	GLuint uniforms[NUM_UNIFORMS];
};