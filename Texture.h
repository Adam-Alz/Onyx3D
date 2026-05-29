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
	Texture(const char* path, bool rgba = false);
	void ActiveAndBind(unsigned int slot);

	unsigned int ID;
	int width, height;

private:
	const char* path;
	void InitRGBA(const char* path);
};

