#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;
in vec3 Normal;

uniform sampler2D ourTexture;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
in vec3 FragPos;

void main()
{
	float ambientCo = 0.1;
	vec3 ambient = ambientCo * lightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diffCo = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diffCo * lightColor;

	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 h = normalize(viewDir + lightDir);
	float specCo = pow(max(dot(h, norm), 0.0), 32);
	vec3 specular = specularStrength * specCo * lightColor;

	float t = 1;
	//vec3 result = (norm);
	vec3 result = (ambient+diffuse+specular) * objectColor;// * objectColor;
	FragColor = vec4(result, 1.0f) * texture(ourTexture, TexCoord);
	//FragColor = t * vec4(result, 1.0f) + (1-t) * texture(ourTexture, TexCoord);
}