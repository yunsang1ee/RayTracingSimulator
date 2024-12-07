#version 460 core

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform vec2 viewportSize;

out vec4 FragColor;

struct Ray {
    vec3 origin;
    vec3 dir;
};

void main()
{
    // 화면 좌표를 클립 공간으로 변환
    vec4 clipSpace = vec4((gl_FragCoord.xy / viewportSize * 2.0f - 1.0f), 1.0f, 1.0f);
    
    // 클립 공간을 뷰 공간으로 변환
    vec4 viewSpace = inverse(projectionMatrix) * clipSpace;
    viewSpace = vec4(viewSpace.xy, -1.0f, 0.0f);
    
    // 뷰 공간을 월드 공간으로 변환
    vec3 worldSpace = (inverse(viewMatrix) * viewSpace).xyz;
    
    Ray ray;
    ray.origin = vec3(0.0f, 0.0f, 0.0f);
    ray.dir = normalize(worldSpace - ray.origin);
    
    // 결과를 리턴
    FragColor = vec4(ray.dir, 1.0f);
    //FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}


//#version 460 core
//
//struct VertexOut
//{
//	vec4 position;
//	vec2 tex;
//	vec3 normal;
//};
//
//struct Light
//{
//	vec3 position;
//	vec3 color;
//};
//
//in VertexOut outData;
//
//uniform bool isTexture;
//uniform bool isAmbient;
//uniform sampler2D textureSampler;
//
//uniform vec3 viewPos;
//uniform Light lights[10];
//uniform int lightCount;
//
//uniform vec4 objectColor;
//
//out vec4 fragColor;
//
//void main()
//{    
//	vec4 texColor;
//	if(isTexture) texColor = texture(textureSampler, outData.tex);
//	else if(objectColor != vec4(0.0f, 0.0f, 0.0f, 0.0f)) texColor = objectColor;
//	else texColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
//
//	vec3 result = texColor.rgb;
//				
//	for (int i = 0; i < lightCount; ++i)
//	{
//		vec3 lightPos = lights[i].position;
//		vec3 lightColor = normalize(lights[i].color);
//
//		float dist = max(distance(lightPos, outData.position.xyz),1);
//
//		float ambientLight;
//		if(isAmbient) ambientLight = 0.5f;
//		else ambientLight = 0.0f;
//		vec3 ambient = ambientLight * texColor.rgb;
//
//		vec3 norm = normalize(outData.normal);
//		vec3 lightDir = normalize(lightPos - outData.position.xyz);
//		float diffuseLight = max(dot(norm, lightDir), 0.0);
//		vec3 diffuse = diffuseLight * mix(texColor.rgb, lightColor, 0.4f);
//
//		int shininess = 16;
//		vec3 viewDir = normalize(viewPos - outData.position.xyz);
//		vec3 reflectDir =  reflect(-lightDir, norm);
//		float specularLight = pow(max(dot (viewDir, reflectDir), 0.0), shininess);
//		vec3 specular = specularLight * lightColor * 0.6f;
//
//		vec3 lightInfo = (ambient + diffuse + specular);
//		lightInfo = mix(lightInfo, lightColor, 1 / (dist * dist));
//		if (i > 0) result = mix(result, lightInfo, 0.5);
//		else result += lightInfo;
//	}
//
//	result = clamp(result, 0.0f, 1.0f);
//
//	fragColor = vec4(result, texColor.a);
//}