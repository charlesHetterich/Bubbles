#include "Sprite.h"

Sprite::Sprite(float x, float y, float w, float h, Shader* shader, Animation* firstAnim)
{
	rect = { x, y, w, h };
	Vertex positions[] = {
		{ -0.5f, -0.5f, 0.0f, 1, 1, 1, 0.0f, 1.0f },
		{ 0.5f, -0.5f, 0.0f, 1, 1, 1, 1.0f, 1.0f },
		{ 0.5f, 0.5f, 0.0f, 1, 1, 1, 1.0f, 0.0f },
		{ -0.5f, 0.5f, 0.0f, 1, 1, 1, 0.0f, 0.0f },
	};

	GLuint indicies[] = { 0, 1, 2, 2, 3, 0 };

	mesh = new Mesh(positions, sizeof(positions) / sizeof(positions[0]), indicies, sizeof(indicies) / sizeof(indicies[0]));
	if (firstAnim != nullptr)
		mesh->setTexCoords(firstAnim->getAnimation());

	this->shader = shader;
}

Sprite::~Sprite()
{
}

void Sprite::draw(Animation* animation)
{
	shader->bind();
	shader->update(getTransform(), getColor());
	if (animation->shouldAnimate())
		mesh->setTexCoords(animation->getAnimation());
	mesh->draw();
}

void Sprite::draw(Shader* shader)
{
	shader->bind();
	shader->update(getTransform(), getColor());
	mesh->draw();
}

void Sprite::draw()
{
	shader->bind();
	shader->update(getTransform(), getColor());
	mesh->draw();
}

glm::mat4 Sprite::getTransform()
{
	return glm::scale(glm::vec3(1 / SCREEN_WIDTH, 1 / SCREEN_HEIGHT, 1)) * glm::translate(glm::vec3(rect.x, rect.y, 0)) *glm::scale(glm::vec3(rect.w, rect.h, 1));
}