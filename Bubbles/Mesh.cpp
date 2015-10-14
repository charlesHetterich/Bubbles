#include "Mesh.h"


Mesh::Mesh(Vertex* vertices, unsigned int numVertices, GLuint* indices, unsigned int numIndices)
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> colors;
	std::vector<glm::vec2> texCoords;
	std::vector<GLuint> Indices;

	this->numVertices = numVertices;

	positions.reserve(numVertices);
	colors.reserve(numVertices);
	Indices.reserve(numIndices);
	texCoords.reserve(numVertices);
	for (unsigned int i = 0; i < numVertices; i++)
	{
		positions.push_back(*vertices[i].getPos());
		colors.push_back(*vertices[i].getColor());
		texCoords.push_back(*vertices[i].getTexCoords());
	}
	for (unsigned int i = 0; i < numIndices; i++)
	{
		Indices.push_back(indices[i]);
	}

	drawCount = numIndices;
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	glGenBuffers(NUM_BUFFERS, vbo);

	//add indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[INDICES]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(Indices[0]), &Indices[0], GL_STATIC_DRAW);

	//---------------------------------------------------------------------------
	//POSITION : INDEX 0
	//---------------------------------------------------------------------------
	glBindBuffer(GL_ARRAY_BUFFER, vbo[POSITION]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(positions[0]), &positions[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(POSITION);
	glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//---------------------------------------------------------------------------
	//COLOR : INDEX 1
	//---------------------------------------------------------------------------
	glBindBuffer(GL_ARRAY_BUFFER, vbo[COLOR]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(colors[0]), &colors[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(COLOR);
	glVertexAttribPointer(COLOR, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//---------------------------------------------------------------------------
	//TEXCOORDS : INDEX 2
	//---------------------------------------------------------------------------
	glBindBuffer(GL_ARRAY_BUFFER, vbo[TEXCOORDS]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(texCoords[0]), &texCoords[0], GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(TEXCOORDS);
	glVertexAttribPointer(TEXCOORDS, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//---------------------------------------------------------------------------
	//DONE
	//---------------------------------------------------------------------------

	//okay now we're really done
	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, vbo);
	glDeleteVertexArrays(1, &vao);
}

void Mesh::draw()
{
	glBindVertexArray(vao);

	glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, 0);
	
	glBindVertexArray(0);
}

void Mesh::setColor(float r, float g, float b)
{
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec3), &glm::vec3(r, g, b), GL_STATIC_DRAW);
}

void Mesh::setTexCoords(Rect rect)
{
	std::vector<glm::vec2> texCoords;

	texCoords.push_back(glm::vec2(rect.x, rect.y + rect.h));
	texCoords.push_back(glm::vec2(rect.x + rect.w, rect.y + rect.h));
	texCoords.push_back(glm::vec2(rect.x + rect.w, rect.y));
	texCoords.push_back(glm::vec2(rect.x, rect.y));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[TEXCOORDS]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(texCoords[0]), &texCoords[0], GL_DYNAMIC_DRAW);
}