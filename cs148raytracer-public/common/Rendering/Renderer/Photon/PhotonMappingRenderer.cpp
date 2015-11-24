#include "common/Rendering/Renderer/Photon/PhotonMappingRenderer.h"
#include "common/Scene/Scene.h"
#include "common/Sampling/ColorSampler.h"
#include "common/Scene/Lights/Light.h"
#include "common/Scene/Geometry/Primitives/Primitive.h"
#include "common/Scene/Geometry/Mesh/MeshObject.h"
#include "common/Rendering/Material/Material.h"
#include "common/Intersection/IntersectionState.h"
#include "common/Scene/SceneObject.h"
#include "common/Scene/Geometry/Mesh/MeshObject.h"
#include "common/Rendering/Material/Material.h"
#include "glm/gtx/component_wise.hpp"

#define VISUALIZE_PHOTON_MAPPING 1

PhotonMappingRenderer::PhotonMappingRenderer(std::shared_ptr<class Scene> scene, std::shared_ptr<class ColorSampler> sampler):
    BackwardRenderer(scene, sampler), 
    diffusePhotonNumber(1000000),
    maxPhotonBounces(1000)
{
    srand(static_cast<unsigned int>(time(NULL)));
}

void PhotonMappingRenderer::InitializeRenderer()
{
    // Generate Photon Maps
    GenericPhotonMapGeneration(diffuseMap, diffusePhotonNumber);
    diffuseMap.optimise();
}

void PhotonMappingRenderer::GenericPhotonMapGeneration(PhotonKdtree& photonMap, int totalPhotons)
{
    float totalLightIntensity = 0.f;
    size_t totalLights = storedScene->GetTotalLights();
    for (size_t i = 0; i < totalLights; ++i) {
        const Light* currentLight = storedScene->GetLightObject(i);
        if (!currentLight) {
            continue;
        }
        totalLightIntensity = glm::length(currentLight->GetLightColor());
    }

    // Shoot photons -- number of photons for light is proportional to the light's intensity relative to the total light intensity of the scene.
    for (size_t i = 0; i < totalLights; ++i) {
        const Light* currentLight = storedScene->GetLightObject(i);
        if (!currentLight) {
            continue;
        }

        const float proportion = glm::length(currentLight->GetLightColor()) / totalLightIntensity;
        const int totalPhotonsForLight = static_cast<const int>(proportion * totalPhotons);
        const glm::vec3 photonIntensity = currentLight->GetLightColor() / static_cast<float>(totalPhotonsForLight);
        for (int j = 0; j < totalPhotonsForLight; ++j) {
            Ray photonRay;
            std::vector<char> path;
            path.push_back('L');
            currentLight->GenerateRandomPhotonRay(photonRay);
            TracePhoton(photonMap, &photonRay, photonIntensity, path, 1.f, maxPhotonBounces);
        }
    }
}

void PhotonMappingRenderer::TracePhoton(PhotonKdtree& photonMap, Ray* photonRay, glm::vec3 lightIntensity, std::vector<char>& path, float currentIOR, int remainingBounces)
{
    /*
     * Assignment 7 TODO: Trace a photon into the scene and make it bounce.
     *    
     *    How to insert a 'Photon' struct into the photon map.
     *        Photon myPhoton;
     *        ... set photon properties ...
     *        photonMap.insert(myPhoton);
     */

    assert(photonRay);
    IntersectionState state(0, 0);
    state.currentIOR = currentIOR;
    
    
//    There exist an intersection
    if(remainingBounces < 0){
        return;
    }
    if(storedScene->Trace(photonRay, &state)){
        if(path.size() != 1){ //Insert if path isnt 1
            const glm::vec3 intersectionPoint = state.intersectionRay.GetRayPosition(state.intersectionT);
        
            Photon myPhoton;
            myPhoton.position = intersectionPoint;
            myPhoton.intensity = lightIntensity;
            Ray r = Ray(photonRay->GetRayPosition(state.intersectionT) , -photonRay->GetRayDirection(), photonRay->GetMaxT());
            myPhoton.toLightRay = r;
            photonMap.insert(myPhoton);
            
           
            
            
        }
        //    Russian roulette
        
        
        const MeshObject* hitMeshObject = state.intersectedPrimitive->GetParentMeshObject();
        
        const Material* hitMaterial = hitMeshObject->GetMaterial();
        
        glm::vec3 dReflect = hitMaterial->GetBaseDiffuseReflection();
//        dReflect = glm::normalize(dReflect);
        float maxXY  = std::max(dReflect.x, dReflect.y);
        float Prob = std::max(maxXY, dReflect.z);
        //    if random 0-1 is less than Prob, scatter,
        float rNumber = rand()/(float)RAND_MAX;
        if(rNumber< Prob){
            path.push_back('B');
            float u1 = rand()/(float)RAND_MAX;
            float u2 = rand()/(float)RAND_MAX;
            float radius = std::sqrt(u1);
            float theta = 2.f*PI*u2;
            float x = radius * cosf(theta);
            float y = radius * sinf(theta);
            float z = std::sqrt(1.f - u1);
            glm::vec3 reflectionRay = glm::normalize(glm::vec3(x, y, z));
            
            glm::vec3 n = state.ComputeNormal();
            
            glm::vec3 t = glm::cross(n, glm::vec3(1.f,0.f,0.f));
            float close = glm::dot(glm::vec3(1.f,0.f,0.f),n);
            
            if(std::abs(close) > 0.9f) {
                t = glm::cross(n, glm::vec3(0.f, 1.f, 0.f));
            }
            t = glm::normalize(t);
            glm::vec3 b = normalize(glm::cross(n, t));
            
            
            glm::vec3 ReflectWorld = glm::mat3(t,b,n)*reflectionRay;
            
            
            Ray *reflRay= new Ray(photonRay->GetRayPosition(state.intersectionT) + n*LARGE_EPSILON ,  ReflectWorld, photonRay->GetMaxT());
            
            TracePhoton(photonMap, reflRay , lightIntensity, path,  currentIOR, remainingBounces - 1);
        }

        
    
    }
    
    

    
}

glm::vec3 PhotonMappingRenderer::ComputeSampleColor(const struct IntersectionState& intersection, const class Ray& fromCameraRay) const
{
    glm::vec3 finalRenderColor = BackwardRenderer::ComputeSampleColor(intersection, fromCameraRay);
#if VISUALIZE_PHOTON_MAPPING
    Photon intersectionVirtualPhoton;
    intersectionVirtualPhoton.position = intersection.intersectionRay.GetRayPosition(intersection.intersectionT);

    std::vector<Photon> foundPhotons;
    diffuseMap.find_within_range(intersectionVirtualPhoton, 0.003f, std::back_inserter(foundPhotons));
    if (!foundPhotons.empty()) {
        finalRenderColor += glm::vec3(1.f, 0.f, 0.f);
    }
#endif
    return finalRenderColor;
}

void PhotonMappingRenderer::SetNumberOfDiffusePhotons(int diffuse)
{
    diffusePhotonNumber = diffuse;
}