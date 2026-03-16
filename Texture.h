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

class Texture
{
public:
	Texture(const char* path);

	unsigned int Init(unsigned int ID, const char* path);
	void ActiveAndBind(unsigned int slot, unsigned int ID);

	unsigned int ID;


private:
	const char* path;
};

