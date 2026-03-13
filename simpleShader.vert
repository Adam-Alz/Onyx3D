#version 330 core

layout (location = 0) in vec3 aPos;

uniform bool useTransform;
uniform mat4 transform;

void main()
{
	if (useTransform)
		gl_Position = transform * vec4(aPos, 1.0f);
	else
		gl_Position = vec4(aPos, 1.0f);
}
