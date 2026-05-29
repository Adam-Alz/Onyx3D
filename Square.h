#pragma once

#include "Mesh.h"
#include "Shader.h"

class Square : public Mesh
{
public:
	Square(std::vector<float> vertices, std::vector<unsigned int> indices, Shader shader);
	Square(std::vector<float> vertices, Shader shader);
	void Init();
	void InitTexture();
	void InitColorTexture();
	void Draw();
	void DrawCube();
	

	~Square();
	

	
private:
	std::vector<unsigned int> indices;
	unsigned int EBO;
};

