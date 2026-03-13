#include "Mesh.h"


Mesh::Mesh(std::vector<float> vertices, Shader shader) : vertices(vertices), shader(shader), trans(glm::mat4(1.0f)) {}


void Mesh::Translate(float x, float y, float z)
{
	this->trans = glm::translate(trans, glm::vec3(x, y, z));
}

void Mesh::Rotate(float degres, float x, float y, float z)
{
	this->trans = glm::rotate(trans, degres, glm::vec3(x, y, z));
}

void Mesh::Scale(float x, float y, float z)
{
	this->trans = glm::scale(trans, glm::vec3(x, y, z));
}

void Mesh::SetColor(std::string uniform, float r, float g, float b, float a)
{
	shader.setFloat4(uniform.c_str(), normalize(r), normalize(g), normalize(b), normalize(a));
}

void Mesh::BindTexture(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void Mesh::SetTexture(const char* filePath)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}