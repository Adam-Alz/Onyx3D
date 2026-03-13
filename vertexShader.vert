#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 vertexColor;
out vec2 TexCoord;

uniform float xOffset;
uniform float yOffset;

uniform bool useTransform;
vec4 transformPos;
uniform mat4 transform;

void main()
{
	if (useTransform)
		transformPos = transform * vec4(aPos.x + xOffset, aPos.y + yOffset, aPos.z, 1.0);
	else
		transformPos = vec4(aPos.x + xOffset, aPos.y + yOffset, aPos.z, 1.0);

	gl_Position = transformPos;
	vertexColor = aColor;	//vec3(0.5, 0.0, 0.0);
	TexCoord = aTexCoord;
}