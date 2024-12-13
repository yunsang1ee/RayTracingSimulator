#version 460 core

uniform mat4 invProjectionMatrix;
uniform mat4 invViewMatrix;
uniform vec3 viewPosition;
uniform vec2 viewportSize;
uniform uint maxBounceCount;
uniform uvec2 screenSize;
uniform uint numRenderedFrame;

out vec4 FragColor;

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
	float theta = 2 * 3.1415926 + RandomValue(state);
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
	
//	HitInfo hitInfo = CalculateRayCollision(ray);
//	if (hitInfo.isHit)
//		incomingLight += hitInfo.material.color.xyz;
	for (int i = 0; i <= maxBounceCount; ++i)
	{
		HitInfo hitInfo = CalculateRayCollision(ray);
		if (hitInfo.isHit)
		{
			ray.origin = hitInfo.hitPoint;
			ray.dir = RandomHemisphereDirection(hitInfo.normal, rngState);

			Material material = hitInfo.material;
			vec3 emittedLight = (material.emittedColor * material.emissionStrength).xyz;
			incomingLight += emittedLight * rayColor;
			rayColor *= material.color.xyz;
		}
	}

	return incomingLight;
}

void main()
{
	// 화면 좌표를 클립 공간으로 변환
	vec4 clipSpace = vec4((gl_FragCoord.xy / viewportSize * 2.0f - 1.0f), 1.0f, 1.0f); // ?
	
	// 클립 공간을 뷰 공간으로 변환
	vec4 viewSpace = invProjectionMatrix * clipSpace;

	viewSpace /= viewSpace.w;

	//viewSpace = vec4(viewSpace.xy, -1.0f, 0.0f);
	
	// 뷰 공간을 월드 공간으로 변환
	vec3 worldSpace = (invViewMatrix * vec4(viewSpace.xyz,1.0)).xyz;

	Ray ray;
	ray.origin = viewPosition;
	ray.dir = normalize(worldSpace - ray.origin);

	
	// 구체 교차 검사
	uvec2 numPixel = screenSize;
	uvec2 pixelCoord = uvec2(gl_FragCoord.xy);
	uint pixelIndex = (pixelCoord.y * numPixel.x + pixelCoord.x);

	uint rngState = pixelIndex + numRenderedFrame;

	FragColor = vec4(Trace(ray, rngState), 1.0f);
	//FragColor = vec4(vec3(RandomDirection(rngState)), 1.0f);
}