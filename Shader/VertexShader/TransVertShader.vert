#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model_Matrix;

void main()
{
	gl_Position = model_Matrix * vec4(aPos, 1.0f);
	TexCoord = aTexCoord;
}