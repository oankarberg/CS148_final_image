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

#define VISUALIZE_PHOTON_MAPPING 0

PhotonMappingRenderer::PhotonMappingRenderer(std::shared_ptr<class Scene> scene, std::shared_ptr<class ColorSampler> sampler):
    BackwardRenderer(scene, sampler), 
    diffusePhotonNumber(400000),
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
        totalLightIntensity += glm::length(currentLight->GetLightColor());
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
    std::cout << "Photon Generation Done. Photons absorbed: " << photonMap.size() << std::endl;
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
    if (remainingBounces < 0) {
        return;
    }
    IntersectionState state(0, 0);
    state.currentIOR = currentIOR;
    
    if (storedScene->Trace(photonRay, &state)){
        
        const MeshObject* hitMeshObject = state.intersectedPrimitive->GetParentMeshObject();
        
        if (hitMeshObject->GetName() == "window_1" || hitMeshObject->GetName() == "window_2" || !hitMeshObject->GetMaterial()->isAffectedByLight()){
            Ray newRay(photonRay->GetRayPosition(state.intersectionT)+ photonRay->GetRayDirection()*0.05f, photonRay->GetRayDirection(), photonRay->GetMaxT());
            
            
            TracePhoton(photonMap, &newRay, lightIntensity, path, currentIOR, remainingBounces);
            return;
        }
        if(path.size() != 1){
            const glm::vec3 intersectionPoint = state.intersectionRay.GetRayPosition(state.intersectionT);
            Photon myPhoton;
            myPhoton.position = intersectionPoint;
            myPhoton.intensity = lightIntensity;
            Ray ray = Ray(photonRay->GetRayPosition(state.intersectionT), -photonRay->GetRayDirection(), photonRay->GetMaxT());
            myPhoton.toLightRay = ray;
            photonMap.insert(myPhoton);
        }
        
        const Material* hitMaterial = hitMeshObject->GetMaterial();
        glm::vec3 ref = hitMaterial->GetBaseDiffuseReflection();
        float pr = std::max(ref.x, ref.y);
        pr = std::max(pr, ref.z);
        float r = rand()/(float)RAND_MAX;
        
        if (r < pr+0.1f) {
            //Scatter
            
            path.push_back('R');
            float u1 = rand()/(float)RAND_MAX;
            float u2 = rand()/(float)RAND_MAX;
            float r = sqrt(u1);
            float theta = 2*PI * u2;
            glm::vec3 refDir = glm::vec3(r*cos(theta), r*sin(theta), sqrt(1.0f-u1));
            refDir = glm::normalize(refDir);
            glm::vec3 n = glm::normalize(state.ComputeNormal());
            glm::vec3 t;
            if (std::abs(glm::dot(n, glm::vec3(1.f,0.f,0.f))) < 0.90)
                t = glm::normalize(glm::cross(n, glm::vec3(1.f,0.f,0.f)));
            else
                t = glm::normalize(glm::cross(n, glm::vec3(0.f,1.f,0.f)));
            glm::vec3 b = glm::normalize(glm::cross(n, t));
            glm::mat3 tbn = glm::mat3(t,b,n);
            refDir = tbn * refDir;
            Ray refRay = Ray(photonRay->GetRayPosition(state.intersectionT)+ n*LARGE_EPSILON, refDir, photonRay->GetMaxT());
            TracePhoton(photonMap, &refRay, lightIntensity, path, currentIOR, remainingBounces-1);
        }
        
        
    }
    
    

    
}

glm::vec3 PhotonMappingRenderer::ComputeSampleColor(const struct IntersectionState& intersection, const class Ray& fromCameraRay) const
{
    glm::vec3 finalRenderColor = BackwardRenderer::ComputeSampleColor(intersection, fromCameraRay);
    //glm::vec3 finalRenderColor = glm::vec3(0.f,0.f,0.f);
    glm::vec3 indirectColor = glm::vec3(0.f,0.f,0.f);
    
    #define FG 1
    const int FG_RAYS =64;
    const float MULTIPLIER = 80.f;
    const float RADIUS = 0.03f;
    
    
    const MeshObject* intersectionObject = intersection.intersectedPrimitive->GetParentMeshObject();
    const Material* intersectionMaterial = intersectionObject->GetMaterial();
    if(!intersectionMaterial->isAffectedByLight()){
        
    }
    #if FG
    
    for (int i= 0; i < FG_RAYS; ) {
        //Sample a random ray from hemisphere
        float u1 = rand()/(float)RAND_MAX;
        float u2 = rand()/(float)RAND_MAX;
        float r = sqrt(u1);
        float theta = 2*PI * u2;
        glm::vec3 refDir = glm::vec3(r*cos(theta), r*sin(theta), sqrt(1.0f-u1));
        refDir = glm::normalize(refDir);
        
        glm::vec3 n = glm::normalize(intersection.ComputeNormal());
        glm::vec3 t;
        
        if (std::abs(glm::dot(n, glm::vec3(1.f,0.f,0.f))) < 0.90)
            t = glm::normalize(glm::cross(n, glm::vec3(1.f,0.f,0.f)));
        else
            t = glm::normalize(glm::cross(n, glm::vec3(0.f,1.f,0.f)));
        glm::vec3 b = glm::normalize(glm::cross(n, t));
        glm::mat3 tbn = glm::mat3(t,b,n);
        
        refDir = tbn * refDir;
        Ray refRay(intersection.intersectionRay.GetRayPosition(intersection.intersectionT)+ n*LARGE_EPSILON, refDir);
        
        IntersectionState state(0, 0);
        
        i++;
        if(storedScene->Trace(&refRay, &state)){
            //i++;
            Photon intersectionVirtualPhoton;
            intersectionVirtualPhoton.position = state.intersectionRay.GetRayPosition(state.intersectionT);
            const MeshObject* parentObject = state.intersectedPrimitive->GetParentMeshObject();
            const Material* objectMaterial = parentObject->GetMaterial();
            std::vector<Photon> foundPhotons;
            diffuseMap.find_within_range(intersectionVirtualPhoton, RADIUS, std::back_inserter(foundPhotons));
            
            glm::vec3 finalGatherColor = glm::vec3(0.f);
            for (Photon photon : foundPhotons) {
                
                const glm::vec3 brdfResponse = objectMaterial->ComputeBRDF(state, photon.intensity, photon.toLightRay, fromCameraRay, 1.0f);
                
                finalGatherColor = finalGatherColor + brdfResponse;
                if (glm::length(finalGatherColor) > 0.05f)
                    glm::to_string(finalGatherColor);
            }
            
            const glm::vec3 brdfOrig = intersectionMaterial->ComputeBRDF(intersection, finalGatherColor/(RADIUS*RADIUS*PI), refRay, fromCameraRay, 1.f);
            indirectColor += MULTIPLIER*brdfOrig/(float)(FG_RAYS);

        }
        
    }
    #else
    Photon intersectionVirtualPhoton;
    intersectionVirtualPhoton.position = intersection.intersectionRay.GetRayPosition(intersection.intersectionT);
    const MeshObject* parentObject = intersection.intersectedPrimitive->GetParentMeshObject();
    const Material* objectMaterial = parentObject->GetMaterial();
    std::vector<Photon> foundPhotons;
    diffuseMap.find_within_range(intersectionVirtualPhoton, RADIUS, std::back_inserter(foundPhotons));
    #if VISUALIZE_PHOTON_MAPPING
    if (!foundPhotons.empty()) {
        finalRenderColor += glm::vec3(1.f, 0.f, 0.f);
    }
    #endif
    for (Photon photon : foundPhotons) {
        const glm::vec3 brdfResponse = objectMaterial->ComputeBRDF(intersection, photon.intensity, photon.toLightRay, fromCameraRay, 1.0f);
        indirectColor += MULTIPLIER*(brdfResponse)/(RADIUS*RADIUS*PI);
    }

    #endif
    
    finalRenderColor = finalRenderColor + indirectColor;
    return finalRenderColor;
}



void PhotonMappingRenderer::SetNumberOfDiffusePhotons(int diffuse)
{
    diffusePhotonNumber = diffuse;
}