#pragma once
#include "globalVariablesAndIncludes.h"
#include "Shader.h"
#include "Texture.h"
class Vertex
{
public:
	Vertex(const glm::vec3& pos = { glm::vec3(0, 0, 0) }, const glm::vec3& color = { glm::vec3(0, 0, 0) }, const glm::vec2& texCoords = { glm::vec2(0, 0) })
	{
		this->pos = pos;
		this->color = color;
		this->texCoords = texCoords;
	}

	Vertex(float x, float y, float z, float r, float g, float b, float texX, float texY)
	{
		
		pos = { x, y, z };
		color = { r, g, b };
		texCoords = { texX, texY };
	}

	inline glm::vec3* getPos() { return &pos; }
	inline glm::vec3* getColor() { return &color; }
	inline glm::vec2* getTexCoords() { return &texCoords; }

private:
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoords;
};

class Mesh
{
public:
	Mesh(Vertex* vertices, unsigned int numVertices, GLuint* indices, unsigned int numIndices);
	~Mesh();
	void draw();
	void setColor(float r, float g, float b);
	void setTexCoords(Rect rect);

private:

	enum
	{
		POSITION,
		COLOR,
		TEXCOORDS,
		INDICES,

		NUM_BUFFERS
	};

	GLuint vao;
	GLuint vbo[NUM_BUFFERS];

	int drawCount;
	int numVertices;
};

