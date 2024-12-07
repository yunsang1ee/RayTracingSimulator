#version 460 core

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
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
    vec4 emissionStrength;
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
    vec4 viewSpace = inverse(projectionMatrix) * clipSpace;

    viewSpace /= viewSpace.w;

    //viewSpace = vec4(viewSpace.xy, -1.0f, 0.0f);
    
    // 뷰 공간을 월드 공간으로 변환
    vec3 worldSpace = (inverse(viewMatrix) * vec4(viewSpace.xyz,1.0)).xyz;

    Ray ray;
    ray.origin = (inverse(viewMatrix) * vec4(viewPosition,1.0)).xyz;
    ray.dir = normalize(worldSpace - ray.origin);

    FragColor = vec4(normalize(worldSpace), 1.0f);
    
    // 구체 설정
    vec4 transformed = (vec4(2.0f, 0.0f, 0.0f, 1.0f));
    vec3 sphereCenter1 = transformed.xyz;
    float sphereRadius1 = 1.0f;
    vec3 sphereCenter2 = vec4(0.0f, 0.f, -1.0f, 1.0f).xyz;
    float sphereRadius2 = 1.0f;

    // 구체 교차 검사
    HitInfo hitInfo1 = RaySphere(ray, (vec4(sphereCenter1, 1.0f)).xyz, sphereRadius1);
    HitInfo hitInfo2 = RaySphere(ray, sphereCenter2, sphereRadius2);

    // 조명 설정
    vec3 lightPos = vec3(0.0f, 0.0f, 2.0f);
    vec3 lightColor = vec3(1.0f);
    vec3 ambientColor = vec3(0.1f);
    vec3 viewDir = normalize(ray.origin - (hitInfo1.isHit ? hitInfo1.hitPoint : hitInfo2.hitPoint));

    vec3 color = vec3(0.0f);
    if (hitInfo1.isHit)
    {
        vec3 lightDir = normalize(lightPos - hitInfo1.hitPoint);
        float diff = max(dot(hitInfo1.normal, lightDir), 0.0f);
        vec3 diffuse = diff * lightColor;

        vec3 reflectDir = reflect(-lightDir, hitInfo1.normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32.0f);
        vec3 specular = spec * lightColor;

        color = (ambientColor + diffuse + specular) * vec3(1.0f, 0.0f, 0.0f); // 빨간색 구체
        FragColor = vec4(color, 1.0f);
    }
    if (hitInfo2.isHit)
    {
        vec3 lightDir = normalize(lightPos - hitInfo2.hitPoint);
        float diff = max(dot(hitInfo2.normal, lightDir), 0.0f);
        vec3 diffuse = diff * lightColor;

        vec3 reflectDir = reflect(-lightDir, hitInfo2.normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32.0f);
        vec3 specular = spec * lightColor;

        color = (ambientColor + diffuse + specular) * vec3(0.0f, 0.0f, 1.0f); // 파란색 구체
        FragColor = vec4(color, 1.0f);
    }

}