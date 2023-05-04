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
	// 定向光需要direction
	vec3 direction;

	// 点光需要三个系数
	float constant;
	float linear;
	float quadratic;

	// 聚光需要一个方向和一个切光角，切光角分为内外光切
	vec3 spotDirection;
	float cutOff;
	float outerCutOff;

	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
	vec3 position;
};

uniform Material material;
uniform Light light;
uniform Light DirectLight;
uniform Light PointLight;
uniform Light SpotLight;

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
//  // directional light
//	vec3 ambient = DirectLight.ambient * vec3(texture(material.diffuse, TexCoord));
//
//	vec3 norm = normalize(Normal);
//	vec3 lightDir = normalize(-DirectLight.direction);
//	float diffCo = max(dot(norm, lightDir), 0.0);
//	vec3 diffuse = diffCo * DirectLight.diffuse * vec3(texture(material.diffuse, TexCoord));
//
//	vec3 viewDir = normalize(viewPos - FragPos);
//	vec3 h = normalize(viewDir + lightDir);
//	float specCo = pow(max(dot(h, norm), 0.0), material.shininess);
//	vec3 specular = specCo * DirectLight.specular * vec3(texture(material.specular, TexCoord));
//
//	vec3 result = (ambient+diffuse+specular);
//	FragColor = 1.5 * vec4(result, 1.0f);// * texture(AnimeTexture, TexCoord);

//	// pointlight
//	vec3 ambient = PointLight.ambient * vec3(texture(material.diffuse, TexCoord));
//	
//	vec3 norm = normalize(Normal);
//	vec3 lightDir = normalize(PointLight.position-FragPos);
//	float diffCo = max(dot(norm, lightDir), 0.0);
//	vec3 diffuse = diffCo * PointLight.diffuse * vec3(texture(material.diffuse, TexCoord));
//
//	vec3 viewDir = normalize(viewPos - FragPos);
//	vec3 h = normalize(viewDir + lightDir);
//	float specCo = pow(max(dot(h, norm), 0.0), material.shininess);
//	vec3 specular = specCo * PointLight.specular * vec3(texture(material.specular, TexCoord));
//	
//	float distance = length(PointLight.position-FragPos);
//	float attenuation = 1.0 / (PointLight.constant + PointLight.linear*distance +
//							PointLight.quadratic*distance*distance);
//
//	vec3 result = (ambient+diffuse+specular) * attenuation;
//	FragColor = vec4(result, 1.0f);

	vec3 ambient = SpotLight.ambient * vec3(texture(material.diffuse, TexCoord));
	
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(SpotLight.position-FragPos);
	float diffCo = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diffCo * SpotLight.diffuse * vec3(texture(material.diffuse, TexCoord));

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 h = normalize(viewDir + lightDir);
	float specCo = pow(max(dot(h, norm), 0.0), material.shininess);
	vec3 specular = specCo * SpotLight.specular * vec3(texture(material.specular, TexCoord));
	
	float theta = dot(-lightDir, normalize(SpotLight.spotDirection));
	float epsilon = SpotLight.cutOff - SpotLight.outerCutOff;
	float intensity = clamp((theta - SpotLight.outerCutOff) / epsilon, 0.0, 1.0);

	diffuse *= intensity;
	specular *= intensity;

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0f);

}