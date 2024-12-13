#version 460 core

uniform mat4 invProjectionMatrix;
uniform mat4 invViewMatrix;
uniform vec3 viewPosition;
uniform vec2 viewportSize;

out vec4 FragColor;

struct Ray
{
    vec3 origin;
    vec3 dir;
};

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

    FragColor = vec4(normalize(worldSpace), 1.0f);
    
    // 구체 교차 검사
    //FragColor = CalculateRayCollision(ray).material.color;
    if(spheres.length() == 0) FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}