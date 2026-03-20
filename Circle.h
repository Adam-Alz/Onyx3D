#pragma once
#define M_PI 3.1415926535897932384626433832795
#include "Mesh.h"


class Circle : public Mesh
{
public:

	Circle(Shader shader, int segment, float radius);
	void Init();
	void InitTexture();
	void Draw();


	int segments;
	float radius;
};

