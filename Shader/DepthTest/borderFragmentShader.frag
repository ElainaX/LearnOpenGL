#version 330 core

in vec2 TexCoords;

float near = 0.1;
float far = 50.0;

uniform sampler2D BoxTexture;

void main()
{
	gl_FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}