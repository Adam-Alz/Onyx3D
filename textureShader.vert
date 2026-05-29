#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform bool useTransform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
	if(useTransform)
		gl_Position = projection * view * model * vec4(aPos, 1.0f);
	else
		gl_Position = vec4(aPos, 1.0f);
	ourColor = aColor;
	TexCoord = aTexCoord;
}