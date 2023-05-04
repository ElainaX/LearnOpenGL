// LightMap frag
#version 330 core
#define NR_POINT_LIGHTS 4
struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirLight
{
	// 定向光需要direction
	vec3 direction;

	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
};

struct PointLight
{
	// 点光需要三个系数
	float constant;
	float linear;
	float quadratic;

	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
	vec3 position;
};

struct SpotLight
{
	// 聚光需要一个方向和一个切光角，切光角分为内外光切
	vec3 direction;
	float cutOff;
	float outerCutOff;

	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
	vec3 position;
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
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;
in vec3 Normal;

uniform sampler2D AnimeTexture;
uniform vec3 objectColor;
uniform vec3 viewPos;
in vec3 FragPos;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 FragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 FragPos, vec3 viewDir);

void main()
{
	// norm,fragPos,viewPos
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	// 第一阶段：定向光照
	vec3 result = CalcDirLight(dirLight, norm, viewDir);
	// 第二阶段：点光源
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
	}
	// 第三阶段：TODO 聚光
	result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	// diffuse
	float diff = max(dot(lightDir, normal), 0.0);
	// specular
	vec3 h = normalize(viewDir + lightDir);
	float spec = pow(max(dot(h, normal), 0.0), material.shininess);
	// 合并结果
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
	return (ambient + diffuse + specular);

}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 FragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - FragPos);
	// diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	// specular
	vec3 h = normalize(lightDir + viewDir);
	float spec = pow(max(dot(h, normal), 0.0), material.shininess);
	// 衰减
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear*distance + 
								light.quadratic*distance*distance);
	// 合并
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
	return (ambient + diffuse + specular) * attenuation;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 FragPos, vec3 viewDir)
{
	
	vec3 lightDir = normalize(light.position-FragPos);
	float diffCo = max(dot(normal, lightDir), 0.0);

	vec3 h = normalize(viewDir + lightDir);
	float specCo = pow(max(dot(h, normal), 0.0), material.shininess);
	
	float theta = dot(-lightDir, normalize(light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

//	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
//	vec3 diffuse = diffCo * light.diffuse * vec3(texture(material.diffuse, TexCoord));
//	vec3 specular = specCo * light.specular * vec3(texture(material.specular, TexCoord));

	float t = 1;
	vec3 ambient = 0.1 * (t * vec3(texture(AnimeTexture, TexCoord)) + (1-t) * vec3(texture(material.diffuse, TexCoord)));
	vec3 diffuse = 0.5 * (t * vec3(texture(AnimeTexture, TexCoord))  + (1-t) * vec3(texture(material.diffuse, TexCoord)));
	vec3 specular = 0.3 * (t * vec3(texture(AnimeTexture, TexCoord)) + (1-t) * vec3(texture(material.specular, TexCoord)));


	return (ambient + diffuse + specular) * intensity;
}

