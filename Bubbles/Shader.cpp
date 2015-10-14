#include "Shader.h"

Shader::Shader(std::string filePath)
{
	//---------------------------------------------------------------------------
	//CREATE PROGRAM AND ADD SHADERS
	//---------------------------------------------------------------------------

	shaderProgram = glCreateProgram();
	vertexShader = createShader(loadShader("./res/shaders/" + filePath + "/vertex.vsh"), GL_VERTEX_SHADER);
	fragmentShader = createShader(loadShader("./res/shaders/" + filePath + "/fragment.fsh"), GL_FRAGMENT_SHADER);
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	/*//---------------------------------------------------------------------------
	//ATTRIBUTES
	//---------------------------------------------------------------------------
	glBindAttribLocation(shaderProgram, 0, "position");
	glBindAttribLocation(shaderProgram, 1, "rgbColor");
	glBindAttribLocation(shaderProgram, 2, "texCoords");

	//---------------------------------------------------------------------------
	//LINK PROGRAM
	//---------------------------------------------------------------------------
	glLinkProgram(shaderProgram);
	CheckShaderError(shaderProgram, GL_LINK_STATUS, true, "Error: Program linking failed: ");
	glValidateProgram(shaderProgram);
	CheckShaderError(shaderProgram, GL_VALIDATE_STATUS, true, "Error: Program is invalid: ");

	//---------------------------------------------------------------------------
	//UNIFORMS
	//---------------------------------------------------------------------------
	//transform
	uniforms[TRANSFORM] = glGetUniformLocation(shaderProgram, "transform");

	//color
	uniforms[OVERRIDE_COLOR] = glGetUniformLocation(shaderProgram, "overrideColor");
	glUniform3f(uniforms[OVERRIDE_COLOR], 1.0f, 1.0f, 1.0f);

	//pixilazation
	uniforms[PIXILATION] = glGetUniformLocation(shaderProgram, "pixilation");

	//clock
	uniforms[CLOCK] = glGetUniformLocation(shaderProgram, "clock");*/
}

Shader::~Shader()
{
	glDetachShader(shaderProgram, vertexShader);
	glDeleteShader(vertexShader);
	glDetachShader(shaderProgram, fragmentShader);
	glDeleteShader(fragmentShader);
}

void Shader::bind()
{
	if (CURRENT_SHADER != shaderNumber)
	{
		glUseProgram(shaderProgram);
		CURRENT_SHADER = shaderNumber;
	}
}

GLuint Shader::getProgram()
{
	return shaderProgram;
}