#include "common/Scene/Lights/Directional/DirectionalLight.h"

void DirectionalLight::ComputeSampleRays(std::vector<Ray>& output, glm::vec3 origin, glm::vec3 normal) const
{
    const glm::vec3 rayDirection = -1.f * glm::vec3(GetForwardDirection());
    output.emplace_back(origin + normal * LARGE_EPSILON, rayDirection);
}

float DirectionalLight::ComputeLightAttenuation(glm::vec3 origin) const
{
    return 1.f;
}

void DirectionalLight::GenerateRandomPhotonRay(Ray& ray) const
{
    glm::vec3 n = glm::vec3(GetForwardDirection());
    glm::vec3 pos = glm::vec3(GetPosition());
    float u1 = rand()/(float)RAND_MAX;
    float u2 = rand()/(float)RAND_MAX;
    
    float r = 10.f*u1;
    float theta = 2*PI * u2;
    glm::vec3 t;
    if (std::abs(glm::dot(n, glm::vec3(1.f,0.f,0.f))) < 0.90)
        t = glm::normalize(glm::cross(n, glm::vec3(1.f,0.f,0.f)));
    else
        t = glm::normalize(glm::cross(n, glm::vec3(0.f,1.f,0.f)));
    glm::vec3 b = glm::normalize(glm::cross(n, t));
    float x = r*cos(theta);
    float y = r*sin(theta);
    
    x = ((rand())/(float)RAND_MAX + (-0.5f))*2.f * 1.f;
    y = ((rand())/(float)RAND_MAX + (-0.5f))*2.f * 8.f;
    glm::vec3 photonPos = pos + r*x + b*y;
    
    ray.SetPosition(photonPos);
    ray.SetRayDirection(n);
    
}