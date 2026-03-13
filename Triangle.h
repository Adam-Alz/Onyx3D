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

#include "Mesh.h"

class Triangle : public Mesh
{
public:
	Triangle(std::vector<float> vertices, Shader shader);

	void Init();
	void InitTexture();
	void InitColorTexture();
	void Draw();


	~Triangle();

};

