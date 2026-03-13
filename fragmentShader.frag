#version 330 core

out vec4 FragColor;

in vec3 vertexColor;
in vec2 TexCoord;

uniform bool useTexture;
uniform bool mixTexture;
uniform float mixValue;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	if (useTexture)
		if (mixTexture)
		//FragColor = texture(ourTexture, TexCoord) * vec4(vertexColor, 1.0); //vec4(vertexColor, 1.0);
			FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixValue);
		else
			FragColor = texture(texture1, TexCoord) * vec4(vertexColor, 1.0);
	else
		FragColor = vec4(vertexColor, 1.0f);
}