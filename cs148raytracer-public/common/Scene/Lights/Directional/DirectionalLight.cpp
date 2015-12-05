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
    
    float u1 = rand()/(float)RAND_MAX;
    float xdelta, ydelta, zdelta = 0;
    
    glm::vec3 windowPos;
    glm::vec3 photonPos;
    ydelta = (rand()/(float)RAND_MAX - 0.5f) * 2.f * 2.5f;
    
    if (u1 < 0.5f){
        windowPos = glm::vec3(5.73f,1.84f,-2.9f);
        zdelta = (rand()/(float)RAND_MAX - 0.5f) * 2.f * 4.5f;
        photonPos = windowPos + glm::vec3(0.f,ydelta,zdelta);
    }else{
        windowPos = glm::vec3(0.22f,1.84f,-8.0f);
        xdelta = (rand()/(float)RAND_MAX - 0.5f) * 2.f * 4.5f;
        photonPos = windowPos + glm::vec3(xdelta,ydelta,0.f);

    }
    
    ray.SetRayDirection(n);
    ray.SetRayPosition(photonPos);
    
}