#include "Circle.h"


Circle::Circle(Shader shader, int segments, float radius) : segments(segments), radius(radius),  Mesh(vertices, shader) {}

void Circle::Init()
{
	this->vertices.push_back(0.0f);
	this->vertices.push_back(0.0f);
	this->vertices.push_back(0.0f);

	for (int i = 0; i <= this->segments; i++)
	{
		float angle = 2.0f * M_PI * i / this->segments;

		float x = radius * cos(angle);
		float y = radius * sin(angle);

		this->vertices.push_back(x);
		this->vertices.push_back(y);
		this->vertices.push_back(0.0f);
	}

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float), this->vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	this->trans = glm::mat4(1.0f);

}


void Circle::Draw()
{
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, segments + 2);
}