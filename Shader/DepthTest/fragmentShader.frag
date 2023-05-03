#version 330 core

in vec2 TexCoords;

float near = 0.1;
float far = 50.0;

uniform sampler2D BoxTexture;

float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0;
	return (2.0*near*far) / (far + near - z * (far - near));
}

void main()
{
	gl_FragColor = texture(BoxTexture, TexCoords);
}