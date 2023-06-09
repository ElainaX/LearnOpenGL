#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aNormal;

uniform mat4 MVP;
uniform mat4 model;
out vec2 TexCoord;
out vec3 ourColor;
out vec3 Normal;
out vec3 FragPos;

void main()
{
	gl_Position = MVP * vec4(aPos, 1.0f);
	FragPos = vec3(model * vec4(aPos, 1.0f));
	TexCoord = aTexCoord;
	ourColor = aColor;
	Normal = vec3(model * vec4(aNormal, 0.0f));
}