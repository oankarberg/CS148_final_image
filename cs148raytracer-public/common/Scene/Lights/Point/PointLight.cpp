#include "common/Scene/Lights/Point/PointLight.h"


void PointLight::ComputeSampleRays(std::vector<Ray>& output, glm::vec3 origin, glm::vec3 normal) const
{
    origin += normal * LARGE_EPSILON;
    const glm::vec3 lightPosition = glm::vec3(GetPosition());
    const glm::vec3 rayDirection = glm::normalize(lightPosition - origin);
    const float distanceToOrigin = glm::distance(origin, lightPosition);
    output.emplace_back(origin, rayDirection, distanceToOrigin);
}

float PointLight::ComputeLightAttenuation(glm::vec3 origin) const
{
    return 1.f;
}

void PointLight::GenerateRandomPhotonRay(Ray& ray) const
{
    // Assignment 7 TODO: Fill in the random point light samples here.
    ray.SetRayPosition(glm::vec3(GetPosition()));
    float x,y, z = 0;
    do{
        x = ((rand())/(float)RAND_MAX + (-0.5f))*2.f;
        y = ((rand())/(float)RAND_MAX + (-0.5f))*2.f;
        z = ((rand())/(float)RAND_MAX + (-0.5f))*2.f;
//        std::cout << x << y << z << " x*x + y*y + z*z) " << ( x*x + y*y + z*z) << std::endl;
    }while((x*x + y*y + z*z) > 1);
    ray.SetRayDirection(glm::vec3(x, y, z));
    
}