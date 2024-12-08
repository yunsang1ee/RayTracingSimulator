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

struct HitInfo
{
    bool isHit;
    float dist;
    vec3 hitPoint;
    vec3 normal;
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

void main()
{
    // ȭ�� ��ǥ�� Ŭ�� �������� ��ȯ
    vec4 clipSpace = vec4((gl_FragCoord.xy / viewportSize * 2.0f - 1.0f), 1.0f, 1.0f);
    
    // Ŭ�� ������ �� �������� ��ȯ
    vec4 viewSpace = inverse(projectionMatrix) * clipSpace;
    viewSpace = vec4(viewSpace.xy, -1.0f, 0.0f);
    
    // �� ������ ���� �������� ��ȯ
    vec3 worldSpace = viewPosition + (inverse(viewMatrix) * viewSpace).xyz;
    FragColor = vec4(normalize(worldSpace), 1.0f);
    Ray ray;
    ray.origin = viewPosition;
    ray.dir = normalize(worldSpace - ray.origin);
    
    // ��ü ����
    vec4 transformed = (projectionMatrix * viewMatrix * vec4(2.0f, 0.0f, 0.0f, 1.0f));
    vec3 sphereCenter1 = transformed.xyz / (transformed.w == 0 ? 1 : transformed.w);
    float sphereRadius1 = 0.1;
    vec3 sphereCenter2 = vec4(2.0f, 0.0f, -1.5f, 1.0f).xyz;
    float sphereRadius2 = 0.1;

    // ��ü ���� �˻�
    HitInfo hitInfo1 = RaySphere(ray, (inverse(viewMatrix) * inverse(projectionMatrix) * vec4(sphereCenter1, 1.0f)).xyz, sphereRadius1);
    HitInfo hitInfo2 = RaySphere(ray, sphereCenter2, sphereRadius2);

    // ���� ����
    vec3 lightPos = vec3(0.0f, 0.0f, 2.0f);
    vec3 lightColor = vec3(1.0f);
    vec3 ambientColor = vec3(0.1f);
    vec3 viewDir = normalize(viewPosition - (hitInfo1.isHit ? hitInfo1.hitPoint : hitInfo2.hitPoint));

    vec3 color = vec3(0.0f);
    if (hitInfo1.isHit)
    {
        vec3 lightDir = normalize(lightPos - hitInfo1.hitPoint);
        float diff = max(dot(hitInfo1.normal, lightDir), 0.0f);
        vec3 diffuse = diff * lightColor;

        vec3 reflectDir = reflect(-lightDir, hitInfo1.normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32.0f);
        vec3 specular = spec * lightColor;

        color = (ambientColor + diffuse + specular) * vec3(1.0f, 0.0f, 0.0f); // ������ ��ü
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

        color = (ambientColor + diffuse + specular) * vec3(0.0f, 0.0f, 1.0f); // �Ķ��� ��ü
        FragColor = vec4(color, 1.0f);
    }

}