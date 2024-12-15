#version 460 core
layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

uniform mat4 invProjectionMatrix;
uniform mat4 invViewMatrix;
uniform vec3 viewPosition;
uniform vec2 viewportSize;
uniform uint maxBounceCount;
uniform uvec2 screenSize;
uniform uint numRenderedFrame;
uniform uint rayPerPixel;

layout (rgba8, binding = 1) uniform image2D outputImage;

struct Ray
{
	vec3 origin;
	vec3 dir;
};

struct Material
{
	vec4 color;             // 16
	vec4 emittedColor;		// 16
	float emissionStrength;	// 4	+ 12
	float smoothness;
	float specularProbability;
	int flag;
};

struct Sphere
{
	vec3 center;        // 12	
	float radius;       // 4	
	Material material;	// 48
};	//64

struct HitInfo
{
	bool isHit;
	float dist;
	vec3 hitPoint;
	vec3 normal;
	Material material;
};

layout(std430, binding = 0) buffer Spheres // std430 -> float = 4byte, vec3 = 12byte, vec4 = 16byte
{
	Sphere spheres[];
};

float RandomValue(inout uint state)
{
	state = state * 747796405 + 2891336453;
	uint result = ((state >> ((state >> 28) + 4)) ^ state) * 277803737;
	result = (result >> 22) ^ result;
	return result / 4294967295.0;
}

float RandomValueNormalDistribution(inout uint state) // Box-Muller transform
{
	float theta = 2 * 3.1415926 * RandomValue(state);
	float rho = sqrt(-2 * log(RandomValue(state)));
	return rho * cos(theta);
}

vec3 RandomDirection(inout uint state)
{
	float x = RandomValueNormalDistribution(state);
	float y = RandomValueNormalDistribution(state);
	float z = RandomValueNormalDistribution(state);
	return normalize(vec3(x, y, z));
}

vec3 RandomHemisphereDirection(vec3 normal, inout uint rngState)
{
	vec3 dir = RandomDirection(rngState);
	return dir * sign(dot(normal, dir));
}

HitInfo RaySphere(Ray ray, vec3 sphereCenter, float sphereRadius)
{
	HitInfo hitInfo;
	hitInfo.isHit = false;
	vec3 offsetRayOrigin = ray.origin - sphereCenter;

	float a = dot(ray.dir, ray.dir);
	float b = 2 * dot(offsetRayOrigin, ray.dir);
	float c = dot(offsetRayOrigin, offsetRayOrigin) - sphereRadius * sphereRadius;
	float nabla = b * b - 4 * a * c;

	if (nabla >= 0)
	{
		float dst = (-b - sqrt(nabla)) / (2 * a);

		if (dst >= 0)
		{
			hitInfo.isHit = true;
			hitInfo.dist = dst;
			hitInfo.hitPoint = ray.origin + ray.dir * dst;
			hitInfo.normal = normalize(hitInfo.hitPoint - sphereCenter);
		}
	}
	return hitInfo;
}

HitInfo CalculateRayCollision(Ray ray)
{
	HitInfo closestHit;
	closestHit.dist = 1.0f / 0.0f; //INF

	for (int i = 0; i < spheres.length(); ++i)
	{
		Sphere sphere = spheres[i];
		HitInfo hitInfo = RaySphere(ray, sphere.center, sphere.radius);

		if (hitInfo.isHit && hitInfo.dist < closestHit.dist)
		{
			closestHit = hitInfo;
			closestHit.material = sphere.material;
		}
	}
	return closestHit;
}

vec3 Trace(Ray ray, inout uint rngState)
{
	vec3 incomingLight = vec3(0.0f);
	vec3 rayColor = vec3(1.0f);
	
	for (int i = 0; i <= maxBounceCount; ++i)
	{
		HitInfo hitInfo = CalculateRayCollision(ray);
		if (hitInfo.isHit)
		{
			ray.origin = hitInfo.hitPoint;
			ray.dir = normalize(hitInfo.normal + RandomDirection(rngState));

			Material material = hitInfo.material;
			vec3 emittedLight = (material.emittedColor * material.emissionStrength).xyz;
			incomingLight += emittedLight * rayColor;
			rayColor *= material.color.xyz;
		}
		else
		{
//			float skyGradienT = pow(smoothstep(0.0, 0.4, ray.dir.y), 0.35);
//			vec3 skyGradien = mix(vec3(205, 241, 252), vec3(116, 176, 255), skyGradienT);
//			float sun = pow(max(0, dot(ray.dir, sunPosition.xyz)), sunFocus) * sunIntensity;
//
//			float groundToSkyT = smoothstep(-0.01, 0.0, ray.dir.y);
//			float sunMask = int(groundToSkyT >= 1);
//			return mix(vec3(0.6, 0.3, 0.18), skyGradien, groundToSkyT) + sun * sunMask;
//		
//			or......
//			
//			cubemap texture?

			break;
		}
	}

	return incomingLight;
}

void main()
{
	// ȭ�� ��ǥ�� Ŭ�� �������� ��ȯ
	vec4 clipSpace = vec4((gl_GlobalInvocationID.xy / viewportSize * 2.0f - 1.0f), 1.0f, 1.0f); // ?
	
	// Ŭ�� ������ �� �������� ��ȯ
	vec4 viewSpace = invProjectionMatrix * clipSpace;

	viewSpace /= viewSpace.w;

	//viewSpace = vec4(viewSpace.xy, -1.0f, 0.0f);
	
	// �� ������ ���� �������� ��ȯ
	vec3 worldSpace = (invViewMatrix * vec4(viewSpace.xyz,1.0)).xyz;

	Ray ray;
	ray.origin = viewPosition;
	ray.dir = normalize(worldSpace - ray.origin);

	
	// ��ü ���� �˻�
	uvec2 numPixel = screenSize;
	uvec2 pixelCoord = uvec2(gl_GlobalInvocationID.xy);
	uint pixelIndex = (pixelCoord.y * numPixel.x + pixelCoord.x);

	uint rngState = pixelIndex + numRenderedFrame * 925904;
	
	vec3 total = vec3(0.0f);

	for(int rayCount = 0; rayCount < rayPerPixel; ++rayCount)
	{
		total += Trace(ray, rngState);
	}

	total /= rayPerPixel;

	imageStore(outputImage, ivec2(pixelCoord), vec4(total, 1.0f));
}