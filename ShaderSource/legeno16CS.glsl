#version 460 core

layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

uniform mat4 invProjectionMatrix;
uniform mat4 invViewMatrix;
uniform vec3 viewPosition;
uniform vec2 viewportSize;
uniform uint maxBounceCount;
uniform uvec2 screenSize;
uniform uint numRenderedFrame;
uniform uint rayPerPixel;
uniform float jitter;
uniform bool isInvironment;

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

struct PackedVertex
{
	vec4 positionU;	// 12
	vec4 normalV;	// 12
}; // 32 bytes

struct TopLevelAccelerationStructure
{
	vec3 aabbMin;			// 12
	uint beginNodeOffset;	// 4
	vec3 aabbMax;			// 12
	uint beginTriangleOffset;				// 4
	mat4 transform;		// 64
	mat4 invTransform;		// 64
	Material material;			// 48
};// 208 bytes

struct GPUBVHNode
{
	vec3 minBounds;		// 12 bytes
	uint leftChild;			// 4 bytes
	vec3 maxBounds;		// 12 bytes  
	uint triangleIndex;		// 4 bytes (leaf일 때)
	uint triangleCount;		// 4 bytes (leaf일 때)
	uint pad[3];			// 12 bytes
}; // 48 bytes

layout (rgba32f, binding = 0) uniform image2D outputImage;
layout (std430, binding = 1) buffer Spheres // std430 -> float = 4byte, vec3 = 12byte, vec4 = 16byte
{
	Sphere spheres[];
};

layout (std430, binding = 2) buffer Vertices {
    PackedVertex vertices[];
};

layout (std430, binding = 3) buffer TLASInstances {
    TopLevelAccelerationStructure tlasInstances[];
};

layout (std430, binding = 4) buffer BVHNodes {
    GPUBVHNode bvhNodes[];
};

struct Ray
{
	vec3 origin;
	vec3 dir;
};

struct HitInfo
{
	bool isHit;
	float dist;
	vec3 hitPoint;
	vec3 normal;
	Material material;
};

const uint UINT32_MAX = ~0u;// 4294967295

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

vec2 RandomPointInCircle(inout uint state)
{
	float angle = RandomValue(state) * 2 * 3.141592;
	vec2 pointOnCircle = vec2(cos(angle), sin(angle));
	return pointOnCircle * sqrt(RandomValue(state));
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

// Slab method
bool RayAABB(Ray r, vec3 bmin, vec3 bmax, out float tMinOut, out float tMaxOut){
    vec3 invD = 1.0 / r.dir;
    vec3 t0 = (bmin - r.origin) * invD;
    vec3 t1 = (bmax - r.origin) * invD;
    vec3 tsm = min(t0, t1);
    vec3 tbg = max(t0, t1);

    float tMin = max(max(tsm.x, tsm.y), tsm.z);
    float tMax = min(min(tbg.x, tbg.y), tbg.z);
    tMinOut = tMin;
    tMaxOut = tMax;
    return tMax >= max(tMin, 0.0);
}

// Moller-Trumbore
HitInfo RayTriangle(Ray ray, vec3 v0, vec3 v1, vec3 v2, vec3 n0, vec3 n1, vec3 n2)
{
	HitInfo hitInfo;
	hitInfo.isHit = false;
	vec3 edge1 = v1 - v0;
	vec3 edge2 = v2 - v0;
	vec3 pvec = cross(ray.dir, edge2);

	float det = dot(edge1, pvec);
	if (abs(det) < 1e-8)
		return hitInfo;
	float invDet = 1.0f / det;
	vec3 tvec = ray.origin - v0;

	float u = dot(tvec, pvec) * invDet;
	if (u < 0.0f || u > 1.0f)
		return hitInfo;

	vec3 qvec = cross(tvec, edge1);
	float v = dot(ray.dir, qvec) * invDet;
	if (v < 0.0f || u + v > 1.0f)
		return hitInfo;

	float t = dot(edge2, qvec) * invDet;
	if (t > 1e-8)
	{
		hitInfo.isHit = true;
		hitInfo.dist = t;
		hitInfo.hitPoint = ray.origin + ray.dir * t;
    float w = 1.0f - u - v;
    hitInfo.normal = normalize(w * n0 + u * n1 + v * n2);
	}
	return hitInfo;
}

HitInfo TraverseBVH(Ray localRay, uint nodeOffset, uint triOffset, Material material)
{
	HitInfo closestHit;
	closestHit.isHit = false;
	closestHit.dist = 1.0f / 0.0f; //INF

	uint stack[64];
	int stackPtr = 0;
	stack[0] = nodeOffset;

	while (stackPtr >= 0)
	{
		uint currentNodeIndex = stack[stackPtr--];
		GPUBVHNode node = bvhNodes[currentNodeIndex];

		float tMin, tMax;
		if (!RayAABB(localRay, node.minBounds, node.maxBounds, tMin, tMax))
			continue;
		if (tMin > closestHit.dist)
			continue;

		if (node.leftChild == UINT32_MAX) // leaf
		{
			for (uint i = 0; i < node.triangleCount; ++i)
			{
				uint triangleIndex = bvhNodes[currentNodeIndex].triangleIndex + i;
				uint vertexIndex0 = triOffset + triangleIndex * 3;
				uint vertexIndex1 = vertexIndex0 + 1;
				uint vertexIndex2 = vertexIndex0 + 2;

				vec3 v0 = vertices[vertexIndex0].positionU.xyz;
				vec3 v1 = vertices[vertexIndex1].positionU.xyz;
				vec3 v2 = vertices[vertexIndex2].positionU.xyz;

				vec3 n0 = vertices[vertexIndex0].normalV.xyz;
				vec3 n1 = vertices[vertexIndex1].normalV.xyz;
				vec3 n2 = vertices[vertexIndex2].normalV.xyz;

				HitInfo hitInfo = RayTriangle(localRay, v0, v1, v2, n0, n1, n2);
				if (hitInfo.isHit && hitInfo.dist < closestHit.dist)
				{
					closestHit = hitInfo;
					closestHit.material = material;
				}
			}
		}
		else // internal
		{
			if (node.leftChild != UINT32_MAX)
			{
				stack[++stackPtr] = node.leftChild;
				stack[++stackPtr] = node.leftChild + 1;
			}
		}
	}

	return closestHit;
}

HitInfo TraceInstances(Ray worldRay)
{
	HitInfo closestHit;
	closestHit.isHit = false;
	closestHit.dist = 1.0f / 0.0f; //INF

    for (uint i = 0u; i < tlasInstances.length(); ++i){
        TopLevelAccelerationStructure inst = tlasInstances[i];

        float tMin, tMax;
        if (!RayAABB(worldRay, inst.aabbMin, inst.aabbMax, tMin, tMax))
            continue;
        if (tMin > closestHit.dist) continue;

        // 월드 → 로컬
        Ray localRay;
        localRay.origin = (inst.invTransform * vec4(worldRay.origin, 1.0)).xyz;
        localRay.dir    = normalize((inst.invTransform * vec4(worldRay.dir, 0.0)).xyz);

        HitInfo localHit = TraverseBVH(localRay, inst.beginNodeOffset, inst.beginTriangleOffset, inst.material);
        if (localHit.isHit){
            // 로컬 결과 → 월드 재투영
            vec3 worldP = (inst.transform * vec4(localHit.hitPoint, 1.0)).xyz;
            float worldT = length(worldP - worldRay.origin);
            if (worldT < closestHit.dist){
                closestHit = localHit;
                closestHit.dist = worldT;
                closestHit.hitPoint = worldP;
                closestHit.normal = normalize((inst.transform * vec4(localHit.normal, 0.0)).xyz);
            }
        }
    }

  return closestHit;
}

HitInfo TraceSphere(Ray ray)
{
	HitInfo closestHit;
	closestHit.isHit = false;
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

HitInfo CalculateRayCollision(Ray ray)
{
	HitInfo hitInfoSphere = TraceSphere(ray);
	HitInfo hitInfoInstance = TraceInstances(ray);
	if (!hitInfoSphere.isHit) return hitInfoInstance;
	if (!hitInfoInstance.isHit) return hitInfoSphere;
	return (hitInfoSphere.dist < hitInfoInstance.dist) ? hitInfoSphere : hitInfoInstance;
}

vec3 Trace(Ray ray, inout uint rngState)
{
	vec3 incomingLight = vec3(0.0f);
	vec3 rayColor = vec3(1.0f);

	for (int i = 0; i <= maxBounceCount; ++i)
	{
		HitInfo hitInfo = CalculateRayCollision(ray);
		if (!hitInfo.isHit)
		{
			if (isInvironment)
			{
				float skyGradienT = pow(smoothstep(0.0, 0.4, ray.dir.y), 0.35);
				float groundToSkyT = smoothstep(-0.01, 0.0, ray.dir.y);

				vec3 skyGradien = mix(vec3(0.8, 0.94, 0.99), vec3(0.45, 0.69, 1.0), skyGradienT);
				float sun = pow(max(0, dot(ray.dir, spheres[0].center)), 0.01f) * 0.1f;

				vec3 environment = mix(vec3(0.6, 0.3, 0.18), skyGradien, groundToSkyT) + sun * int(groundToSkyT >= 1);

				incomingLight += environment * rayColor;
			}
			break;
		}

		Material material = hitInfo.material;

		vec3 emittedLight = (material.emittedColor * material.emissionStrength).xyz;
		incomingLight += emittedLight * rayColor;

		vec3 diffuseDir = normalize(hitInfo.normal + RandomDirection(rngState));
		vec3 specularDir = reflect(ray.dir, hitInfo.normal);
		specularDir *= sign(dot(hitInfo.normal, specularDir));

		bool isSpecularBounce = (material.specularProbability >= RandomValue(rngState));
		rayColor *= mix(material.color.xyz, vec3(1.0f), isSpecularBounce);

		ray.origin = hitInfo.hitPoint + hitInfo.normal * 0.00005;
		ray.dir = normalize(mix(diffuseDir, specularDir, material.smoothness * float(isSpecularBounce)));
	}
	
	return incomingLight;
}

void main()
{
	uvec2 pixelCoord = uvec2(gl_GlobalInvocationID.xy);
	uint pixelIndex = (pixelCoord.y * screenSize.x + pixelCoord.x);

	uint rngState = pixelIndex ^ (numRenderedFrame * 1664525 + 1013904223);
		
	vec3 total = vec3(0.0f);

	for(int rayCount = 0; rayCount < rayPerPixel; ++rayCount)
	{
		vec2 jit = RandomPointInCircle(rngState) * jitter;
		vec2 screenCoord = (vec2(pixelCoord) + jit + 0.5f) / viewportSize;
		vec4 clipSpace = vec4(screenCoord * 2.0f - 1.0f, 1.0f, 1.0f);
		vec4 viewSpace = invProjectionMatrix * clipSpace;
		viewSpace /= viewSpace.w;

		vec3 worldSpace = (invViewMatrix * vec4(viewSpace.xyz, 1.0f)).xyz;

		Ray ray;
		ray.origin = viewPosition;
		ray.dir = normalize(worldSpace - ray.origin);

		// 현재 환경광이 반사값으로 들어옴 fragment shader와의 차이
		total += Trace(ray, rngState); 
	}

	total /= rayPerPixel;

	imageStore(outputImage, ivec2(gl_GlobalInvocationID.xy), vec4(total, 1.0f));//OK
}