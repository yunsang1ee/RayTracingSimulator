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
    vec4 color;				// 16
    vec4 emittedColor;		// 16
    float emissionStrength;	// 4 + 12
}; // 48

struct Sphere
{
    vec3 center;		// 12 + 4
    float radius;		// 4
    Material material;	// 48
}; //

in VertexOut outData;

uniform bool isTexture;
uniform sampler2D textureSampler;

uniform vec3 viewPos;
//uniform Light lights[10];
//uniform int lightCount;

uniform vec4 objectColor;

out vec4 fragColor;

layout(std430, binding = 0) buffer Spheres
{
    Sphere spheres[];
};

void main()
{    
	vec4 texColor;
	if(isTexture) texColor = texture(textureSampler, outData.tex);
	else if(objectColor != vec4(0.0f)) texColor = objectColor;
	else texColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);

	vec3 result = vec3(0.0f);

    for (int i = 0; i < spheres.length(); ++i)
    {
        if (spheres[i].material.emissionStrength <= 0.0) continue;
        vec3 lightPos = spheres[i].center;
        vec3 lightColor = spheres[i].material.emittedColor.xyz * spheres[i].material.emissionStrength;

        float dist = distance(lightPos, outData.position.xyz);

        if (abs(dist - spheres[i].radius) < 0.1)
        {
            result += lightColor;
            continue;
        }

        vec3 norm = normalize(outData.normal);
        vec3 lightDir = normalize(lightPos - outData.position.xyz);
        vec3 viewDir = normalize(viewPos - outData.position.xyz);
        vec3 reflectDir = reflect(-lightDir, norm);

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * texColor.xyz * lightColor;

        int shininess = 16;
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = spec * lightColor; 

        vec3 lightEffect = (diffuse + specular);
        result += lightEffect;
    }

	result = clamp(result, 0.0f, 1.0f);

	fragColor = vec4(result, texColor.a);
}