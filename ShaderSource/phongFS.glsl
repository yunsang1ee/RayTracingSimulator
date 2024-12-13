#version 460 core

struct VertexOut
{
	vec4 position;
	vec2 tex;
	vec3 normal;
};

//struct Light
//{
//	vec3 position;
//	vec3 color;
//};

struct Material
{
    vec4 color;
    vec4 emittedColor;
    float emissionStrength;
};

struct Sphere
{
    vec3 center;
    float radius;
    Material material;
};

in VertexOut outData;

uniform bool isTexture;
uniform sampler2D textureSampler;

uniform vec3 viewPos;
//uniform Light lights[10];
//uniform int lightCount;

//uniform vec4 objectColor;

out vec4 fragColor;

layout(std430, binding = 0) buffer Spheres
{
    Sphere spheres[];
};

void main()
{    
	vec4 texColor;
	if(isTexture) texColor = texture(textureSampler, outData.tex);
	else texColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);

	vec3 result = vec3(0.0f);
				
	for (int i = 0; i < spheres.length(); ++i)
	{
		if (spheres[i].material.emissionStrength <= 0.0) continue;
		vec3 lightPos = spheres[i].center;
		vec3 lightColor = normalize(spheres[i].material.emittedColor).xyz;

		float dist = max(distance(lightPos, outData.position.xyz),1);

		//float ambientLight;
		//ambientLight = 0.0f;
		//vec3 ambient = ambientLight * texColor.rgb;

		vec3 norm = normalize(outData.normal);
		vec3 lightDir = normalize(lightPos - outData.position.xyz);
		float diffuseLight = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = diffuseLight * mix(texColor.rgb, lightColor, 0.4f);

		int shininess = 16;
		vec3 viewDir = normalize(viewPos - outData.position.xyz);
		vec3 reflectDir =  reflect(-lightDir, norm);
		float specularLight = pow(max(dot (viewDir, reflectDir), 0.0), shininess);
		vec3 specular = specularLight * lightColor * 0.6f;

		//vec3 lightInfo = (ambient + diffuse + specular);
		vec3 lightInfo = (diffuse + specular);
		lightInfo = mix(lightInfo, lightColor, 1 / (dist * dist));
		//if (i > 0) result = mix(result, lightInfo, 0.5);
		//else result += lightInfo;
		result += lightInfo;
	}

	result = clamp(result, 0.0f, 1.0f);

	fragColor = vec4(result, texColor.a);
	if (spheres.length() == 0) fragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}