#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <array>
#include <vector>
#include <string>
#include "Shader.h"
#include "stb_image.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Mesh
{
public:
	Mesh(std::vector<float>vertices, Shader shader);

	void Translate(float x, float y, float z);
	void Rotate(float degres, float x, float y, float z);
	void Scale(float x, float y, float z);
	void SetColor(std::string uniform, float r, float g, float b, float a);
	void SetTexture(const char* filePath);
	void BindTexture(unsigned int slot);

	float normalize(float v)
	{
		return ((v - 0) / (255 - 0));
	}

	glm::mat4 trans;

protected:
	unsigned int VAO, VBO;
	std::vector<float> vertices;
	unsigned int texture;
	Shader shader;
};

