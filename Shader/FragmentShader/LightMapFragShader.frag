// LightMap frag
#version 330 core
struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light
{
	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
	vec3 Pos;
};

uniform Material material;
uniform Light light;

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;
in vec3 Normal;

uniform sampler2D AnimeTexture;
uniform vec3 objectColor;
uniform vec3 viewPos;
in vec3 FragPos;

void main()
{
	//float ambientCo = 0.1;
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.Pos - FragPos);
	float diffCo = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diffCo * light.diffuse * vec3(texture(material.diffuse, TexCoord));

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 h = normalize(viewDir + lightDir);
	float specCo = pow(max(dot(h, norm), 0.0), material.shininess);
	vec3 specular = specCo * light.specular * vec3(texture(material.specular, TexCoord));

	vec3 result = (ambient+diffuse+specular);
	FragColor = 1.5 * vec4(result, 1.0f) * texture(AnimeTexture, TexCoord);
}