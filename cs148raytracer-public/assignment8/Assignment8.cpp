#include "assignment8/Assignment8.h"
#include "common/core.h"

std::shared_ptr<Camera> Assignment8::CreateCamera() const
{
    const glm::vec2 resolution = GetImageOutputResolution();
    std::shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(resolution.x / resolution.y, 26.6f);
    
    //Real scene camera
    
    camera->Rotate(glm::vec3(0.f, 1.f, 0.f), -PI / 4.f);
    camera->SetPosition(glm::vec3(-1.6f, -2.0069f, 0.23693f));
    camera->Rotate(glm::vec3(1.f, 0.f, 0.f), PI / 2.f);
    
    // CORNELL CAMERA
    /*
    camera->SetPosition(glm::vec3(0.f, -4.1469f, 0.73693f));
    camera->Rotate(glm::vec3(1.f, 0.f, 0.f), PI / 2.f);
    */
    return camera;
}

std::shared_ptr<Scene> Assignment8::CreateScene() const
{
    return LoadRealScene();
    //return LoadCornellScene();
}
std::shared_ptr<ColorSampler> Assignment8::CreateSampler() const
{
    std::shared_ptr<JitterColorSampler> jitter = std::make_shared<JitterColorSampler>();
    jitter->SetGridSize(glm::ivec3(1, 1, 1));
    return jitter;
}

std::shared_ptr<class Renderer> Assignment8::CreateRenderer(std::shared_ptr<Scene> scene, std::shared_ptr<ColorSampler> sampler) const
{
    
    return std::make_shared<BackwardRenderer>(scene, sampler);
}

int Assignment8::GetSamplesPerPixel() const
{
    // ASSIGNMENT 5 TODO: Change the '1' here to increase the maximum number of samples used per pixel. (Part 1).
    return 2;
}

bool Assignment8::NotifyNewPixelSample(glm::vec3 inputSampleColor, int sampleIndex)
{
    return true;
}

int Assignment8::GetMaxReflectionBounces() const
{
    return 4;
}

int Assignment8::GetMaxRefractionBounces() const
{
    return 4;
}

glm::vec2 Assignment8::GetImageOutputResolution() const
{
    return glm::vec2(640.f, 480.f);
}


std::shared_ptr<Scene> Assignment8::LoadRealScene() const {
    std::shared_ptr<Scene> newScene = std::make_shared<Scene>();
    
    // Material
    std::shared_ptr<BlinnPhongMaterial> cubeMaterial = std::make_shared<BlinnPhongMaterial>();
    cubeMaterial->SetDiffuse(glm::vec3(1.f, 1.f, 1.f));
    cubeMaterial->SetSpecular(glm::vec3(0.6f, 0.6f, 0.6f), 40.f);
    cubeMaterial->SetAmbient(glm::vec3(0.2f));
    //    cubeMaterial->SetReflectivity(0.3f);
    
    
    // Objects
    std::vector<std::shared_ptr<aiMaterial>> loadedMaterials;
    std::vector<std::shared_ptr<MeshObject>> cubeObjects = MeshLoader::LoadMesh("CornellBox/CornellBox-Empty-RG.obj", &loadedMaterials);
    for (size_t i = 0; i < cubeObjects.size(); ++i) {
        std::shared_ptr<Material> materialCopy = cubeMaterial->Clone();
        materialCopy->LoadMaterialFromAssimp(loadedMaterials[i]);
        materialCopy->SetAmbient(glm::vec3(0.f,0.f,0.f));
        cubeObjects[i]->SetMaterial(materialCopy);
    }
    
    std::shared_ptr<SceneObject> cubeSceneObject = std::make_shared<SceneObject>();
    cubeSceneObject->AddMeshObject(cubeObjects);
    cubeSceneObject->Rotate(glm::vec3(1.f, 0.f, 0.f), PI / 2.f);
    cubeSceneObject->MultScale(1.35f);
    cubeSceneObject->SetPosition(glm::vec3(0.f,0.f,-0.4f));
    cubeSceneObject->CreateAccelerationData(AccelerationTypes::BVH);
    newScene->AddSceneObject(cubeSceneObject);
    
    // Objects
    std::vector<std::shared_ptr<aiMaterial>> waterMaterials;
    std::vector<std::shared_ptr<MeshObject>> waterObjects = MeshLoader::LoadMesh("Water.obj", &waterMaterials);
    for (size_t i = 0; i < waterObjects.size(); ++i) {
        std::shared_ptr<Material> materialCopy = cubeMaterial->Clone();
        materialCopy->LoadMaterialFromAssimp(waterMaterials[i]);
        //        materialCopy->SetAmbient(glm::vec3(0,0,0));
        materialCopy->SetAmbient(glm::vec3(0.0f));
        if (i == 1){
            materialCopy->SetAmbient(glm::vec3(0.1f,0.1f,0.1f));
            materialCopy->SetReflectivity(0.01f);
            materialCopy->SetTransmittance(0.99f);
            
            materialCopy->SetIOR(1.33f);
        }
        if (i == 0){
            //            materialCopy->SetAmbient(glm::vec3(0,0,0));
            materialCopy->SetTransmittance(0.95f);
            materialCopy->SetReflectivity(0.05f);
                        materialCopy->SetIOR(1.55f);
        }
        
        waterObjects[i]->SetMaterial(materialCopy);
    }
    
    std::shared_ptr<SceneObject> waterObject = std::make_shared<SceneObject>();
    waterObject->AddMeshObject(waterObjects);
    waterObject->MultScale(0.1f);
    waterObject->Rotate(glm::vec3(1.f, 0.f, 0.f), PI / 2.f);
    waterObject->SetPosition(glm::vec3(0.f,0.f,0.f));
    waterObject->CreateAccelerationData(AccelerationTypes::BVH);
    newScene->AddSceneObject(waterObject);
    
    
    
    std::vector<std::shared_ptr<aiMaterial>> tableMaterials;
    std::vector<std::shared_ptr<MeshObject>> table = MeshLoader::LoadMesh("table/table.obj", &tableMaterials);
    for (size_t i = 0; i < table.size(); ++i) {
        std::shared_ptr<Material> materialCopy = cubeMaterial->Clone();
        materialCopy->LoadMaterialFromAssimp(tableMaterials[i]);
        materialCopy->SetAmbient(glm::vec3(0.1f,0.1f,0.1f));
        table[i]->SetMaterial(materialCopy);
    }
    
    std::shared_ptr<SceneObject> tableObject = std::make_shared<SceneObject>();
    tableObject->AddMeshObject(table);
    tableObject->MultScale(1.f);
    tableObject->Rotate(glm::vec3(1.f, 0.f, 0.f), PI / 2.f);
    tableObject->CreateAccelerationData(AccelerationTypes::BVH);
    tableObject->SetPosition(glm::vec3(0.f,0.f,0.f));
    newScene->AddSceneObject(tableObject);
    
    
    
    std::vector<std::shared_ptr<aiMaterial>> tetraMaterials;
    std::vector<std::shared_ptr<MeshObject>> tetra = MeshLoader::LoadMesh("tetrahedron.obj", &tetraMaterials);
    for (size_t i = 0; i < tetra.size(); ++i) {
        std::cout << "ÄR jag här?!";
        std::shared_ptr<Material> materialCopy = cubeMaterial->Clone();
        materialCopy->LoadMaterialFromAssimp(tetraMaterials[i]);
        
        materialCopy->SetAmbient(glm::vec3(0.02f,0.02f,0.02f));
        materialCopy->SetTransmittance(0.2f);
        materialCopy->SetReflectivity(0.8f);
        materialCopy->SetIOR(1.6f);
        
        tetra[i]->SetMaterial(materialCopy);
    }
    
    std::shared_ptr<SceneObject> tetraObject = std::make_shared<SceneObject>();
    tetraObject->AddMeshObject(tetra);
    tetraObject->MultScale(0.2f);
    tetraObject->Rotate(glm::vec3(1.f, 0.f, 0.f), PI / 2.f);
    //tetraObject->Rotate(glm::vec3(0.f, 1.f, 0.f), PI / 4.f);
    tetraObject->CreateAccelerationData(AccelerationTypes::BVH);
    tetraObject->SetPosition(glm::vec3(0.6f,0.f,0.0f));
    newScene->AddSceneObject(tetraObject);

    
    //    std::cout << "Cube scen pos "<<  cubeObjects[0]->get << std:: endl;
    
    // Lights
    std::shared_ptr<PointLight> pointLight = std::make_shared<PointLight>();
    //    pointLight->SetPosition(glm::vec3(0.01909f, 0.0101f, 1.97028f));
    pointLight->SetPosition(glm::vec3(-0.005f, -0.01f, 1.5328f));
    pointLight->SetLightColor(glm::vec3(1.0f, 1.0f, 1.0f));
    newScene->AddLight(pointLight);
    
    return newScene;

}

std::shared_ptr<Scene> Assignment8::LoadCornellScene() const {
    std::shared_ptr<Scene> newScene = std::make_shared<Scene>();
    
    // Material
    std::shared_ptr<BlinnPhongMaterial> cubeMaterial = std::make_shared<BlinnPhongMaterial>();
    cubeMaterial->SetDiffuse(glm::vec3(1.f, 1.f, 1.f));
    cubeMaterial->SetSpecular(glm::vec3(0.6f, 0.6f, 0.6f), 40.f);
    
    // Objects
    std::vector<std::shared_ptr<aiMaterial>> loadedMaterials;
    std::vector<std::shared_ptr<MeshObject>> cubeObjects = MeshLoader::LoadMesh("CornellBox/CornellBox-Water.obj", &loadedMaterials);
    for (size_t i = 0; i < cubeObjects.size(); ++i) {
        std::shared_ptr<Material> materialCopy = cubeMaterial->Clone();
        materialCopy->LoadMaterialFromAssimp(loadedMaterials[i]);
        materialCopy->SetAmbient(glm::vec3(0.f,0.f,0.f));
        if (i == 0) {
            materialCopy->SetReflectivity(1.0f);
        }
        if (i == 1) {
            materialCopy->SetTransmittance(0.9f);
        }
        if (i == 8) {
            materialCopy->SetTransmittance(0.99f);
            materialCopy->SetReflectivity(0.01f);
        }
        cubeObjects[i]->SetMaterial(materialCopy);
        
    }
    
    std::shared_ptr<SceneObject> cubeSceneObject = std::make_shared<SceneObject>();
    cubeSceneObject->AddMeshObject(cubeObjects);
    cubeSceneObject->Rotate(glm::vec3(1.f, 0.f, 0.f), PI / 2.f);
    cubeSceneObject->CreateAccelerationData(AccelerationTypes::BVH);
    newScene->AddSceneObject(cubeSceneObject);
    
    //    std::cout << "Cube scen pos "<<  cubeObjects[0]->get << std:: endl;
    
    // Lights
    std::shared_ptr<PointLight> pointLight = std::make_shared<PointLight>();
    pointLight->SetPosition(glm::vec3(-0.005f, -0.01f, 1.5328f));
    pointLight->SetLightColor(glm::vec3(1.f, 1.f, 1.f));
    newScene->AddLight(pointLight);
    
    return newScene;
}