#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

const float offset = 1.0 / 300.0;

void main()
{
	// FragColor = vec4(1-texture(screenTexture, TexCoords).xyz, 1.0);

//	FragColor = texture(screenTexture, TexCoords);
//	float average = (FragColor.x + FragColor.y + FragColor.z) / 3.0;
//	FragColor = vec4(average, average, average, 1.0);

// Kernel
	vec2 offset[9] = vec2[](
		vec2(-offset, offset),
		vec2(0.0f, offset),
		vec2(offset, offset),
		vec2(-offset, 0.0f),
		vec2(-0.0f, 0.0f),
		vec2(offset, 0.0f),
		vec2(-offset, -offset),
		vec2(-0.0f, -offset),
		vec2(offset, -offset)
	);

	// Èñ»¯ºË
	float kernel[9] = float[](
		-1, -1, -1,
		-1, 9, -1,
		-1, -1, -1
	);	
	// Ä£ºýºË
	float kernel2[9] = float[](
		1,  2,  1,
		2, 4, 2,
		1, 2, 1
	);
	// ±ßÔµÈñ»¯
	float kernel3[9] = float[](
		1, 1, 1,
		1, -8, 1,
		1, 1, 1
	);
	vec3 sampleTex[9];
	for(int i = 0; i < 9; ++i)
	{
		sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offset[i]));
	}
	vec3 col = vec3(0.0f);
	for(int i = 0; i < 9; ++i)
	{
		col += sampleTex[i] * kernel3[i] / 16.0;
	}

	FragColor = vec4(col, 1.0f);

}