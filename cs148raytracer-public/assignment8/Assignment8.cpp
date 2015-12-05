#include "assignment8/Assignment8.h"
#include "common/core.h"

std::shared_ptr<Camera> Assignment8::CreateCamera() const
{
    const glm::vec2 resolution = GetImageOutputResolution();
    std::shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(resolution.x / resolution.y, 56.6f);
    
    //Real scene camera
    /* OPTION 1
    camera->Rotate(glm::vec3(0.f, 1.f, 0.f), -PI / 6.f);
    camera->SetPosition(glm::vec3(-0.6f, -1.5069f, 0.53693f));
    camera->Rotate(glm::vec3(1.f, 0.0f, 0.f), PI / 2.f);
     */
    // OPTION 2
//    camera->SetPosition(glm::vec3(0.0f, -2.6069f, 1.17593f));
    //camera->Rotate(glm::vec3(0.f, 1.0f, 0.f), -PI / 16.f);
//    camera->Rotate(glm::vec3(1.f, 0.0f, 0.f), -PI / 20.f);
//     camera->Rotate(glm::vec3(1.f, 0.0f, 0.f), PI / 2.f);
//    camera->SetPosition(glm::vec3(0.f, -4.1469f, 0.73693f));
//    camera->SetPosition(glm::vec3(-10.f, -12.f, 40.f));
//    camera->Rotate(glm::vec3(0.f, 0.f, 1.f), PI / 2.f);
       camera->SetPosition(glm::vec3(-5.4f,0.7f, 6.0f));
//    Camera for livingroom.obj
//    camera->SetPosition(glm::vec3(0.f,3.f, 40.73693f));
   camera->Rotate(glm::vec3(0.f, 1.f, 0.f), -PI / 5.f);
    
    
    // CORNELL CAMERA
    /*
    camera->SetPosition(glm::vec3(0.f, -2.1469f, 0.73693f));
    camera->Rotate(glm::vec3(1.f, 0.f, 0.f), PI / 2.f);
    */
    return camera;
}

std::shared_ptr<Scene> Assignment8::CreateScene() const
{
   return LoadRealScene();
    return LoadCornellScene();
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
    return 1;
}

bool Assignment8::NotifyNewPixelSample(glm::vec3 inputSampleColor, int sampleIndex)
{
    return true;
}

int Assignment8::GetMaxReflectionBounces() const
{
    return 1;
}

int Assignment8::GetMaxRefractionBounces() const
{
    return 1;
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
//    std::vector<std::shared_ptr<MeshObject>> cubeObjects = MeshLoader::LoadMesh("CornellBox/CornellBox-Empty-White.obj", &loadedMaterials);
//    for (size_t i = 0; i < cubeObjects.size(); ++i) {
//        std::shared_ptr<Material> materialCopy = cubeMaterial->Clone();
//        materialCopy->LoadMaterialFromAssimp(loadedMaterials[i]);
//        materialCopy->SetAmbient(glm::vec3(0.f,0.f,0.f));
//        cubeObjects[i]->SetMaterial(materialCopy);
//    }
//    
//    std::shared_ptr<SceneObject> cubeSceneObject = std::make_shared<SceneObject>();
//    cubeSceneObject->AddMeshObject(cubeObjects);
//    cubeSceneObject->Rotate(glm::vec3(1.f, 0.f, 0.f), PI / 2.f);
//    cubeSceneObject->MultScale(1.0f);
//    cubeSceneObject->SetPosition(glm::vec3(0.f,0.f,-0.0f));
//    cubeSceneObject->CreateAccelerationData(AccelerationTypes::BVH);
//    newScene->AddSceneObject(cubeSceneObject);
    
    // Objects
//    std::vector<std::shared_ptr<aiMaterial>> waterMaterials;
//    std::vector<std::shared_ptr<MeshObject>> waterObjects = MeshLoader::LoadMesh("wineglass.obj", &waterMaterials);
//    for (size_t i = 0; i < waterObjects.size(); ++i) {
//        std::shared_ptr<Material> materialCopy = cubeMaterial->Clone();
//        materialCopy->LoadMaterialFromAssimp(waterMaterials[i]);
//        //        materialCopy->SetAmbient(glm::vec3(0,0,0));
//        materialCopy->SetAmbient(glm::vec3(0.0f));
//        if (i == 1){
//            materialCopy->SetAmbient(glm::vec3(0.1f,0.1f,0.1f));
//            materialCopy->SetReflectivity(0.01f);
//            materialCopy->SetTransmittance(0.99f);
//            
//            materialCopy->SetIOR(1.33f);
//        }
//        if (i == 0){
//            //            materialCopy->SetAmbient(glm::vec3(0,0,0));
//            materialCopy->SetTransmittance(0.95f);
//            materialCopy->SetReflectivity(0.05f);
//                        materialCopy->SetIOR(1.55f);
//        }
//        
//        waterObjects[i]->SetMaterial(materialCopy);
//    }
//    
//    std::shared_ptr<SceneObject> waterObject = std::make_shared<SceneObject>();
//    waterObject->AddMeshObject(waterObjects);
//    waterObject->MultScale(0.04f);
//    waterObject->Rotate(glm::vec3(1.f, 0.f, 0.f), PI / 2.f);
//    waterObject->SetPosition(glm::vec3(0.f,0.f,0.43f));
//    waterObject->CreateAccelerationData(AccelerationTypes::BVH);
//    newScene->AddSceneObject(waterObject);
    
//    std::vector<std::shared_ptr<aiMaterial>> tableMaterials;
//    std::vector<std::shared_ptr<MeshObject>> table = MeshLoader::LoadMesh("glass_table/glass_table.obj", &tableMaterials);
//    std::cout <<"  table.size() " <<  table.size() << std::endl;
//    for (size_t i = 0; i < table.size(); ++i) {
////        if(i != 2){
//        std::shared_ptr<Material> materialCopy = cubeMaterial->Clone();
//        materialCopy->LoadMaterialFromAssimp(tableMaterials[i]);
//        materialCopy->SetAmbient(glm::vec3(0.1f,0.1f,0.1f));
//        table[i]->SetMaterial(materialCopy);
//        if(i == 1){
//            materialCopy->SetReflectivity(.3f);
//        	materialCopy->SetTransmittance(.0f);
//        	materialCopy->SetIOR(1.01f);
//        }
////        if(i == 3){
////            //            materialCopy->SetReflectivity(0.4f);
////            materialCopy->SetTransmittance(0.9f);
////            materialCopy->SetIOR(1.5f);
////        }
//    }
    
//    std::shared_ptr<SceneObject> tableObject = std::make_shared<SceneObject>();
//    tableObject->AddMeshObject(table);
//    tableObject->MultScale(.3f);
//    tableObject->Rotate(glm::vec3(0.f, 1.f, 0.f), PI / 2.f);
//    tableObject->Rotate(glm::vec3(1.f, 0.f, 0.f), PI / 2.f);
//    tableObject->CreateAccelerationData(AccelerationTypes::BVH);
//    tableObject->SetPosition(glm::vec3(0.0f,0.2f,0.f));
//    newScene->AddSceneObject(tableObject);
    
    
//    
//    std::vector<std::shared_ptr<aiMaterial>> tetraMaterials;
//    std::vector<std::shared_ptr<MeshObject>> tetra = MeshLoader::LoadMesh("tetrahedron.obj", &tetraMaterials);
//    for (size_t i = 0; i < tetra.size(); ++i) {
//        std::shared_ptr<Material> materialCopy = cubeMaterial->Clone();
//        materialCopy->LoadMaterialFromAssimp(tetraMaterials[i]);
//        
//        materialCopy->SetAmbient(glm::vec3(0.02f,0.02f,0.02f));
//        materialCopy->SetTransmittance(0.3f);
//        materialCopy->SetReflectivity(0.5f);
//        materialCopy->SetIOR(1.6f);
//        
//        tetra[i]->SetMaterial(materialCopy);
//    }
//    
//    std::shared_ptr<SceneObject> tetraObject = std::make_shared<SceneObject>();
//    tetraObject->AddMeshObject(tetra);
//    tetraObject->MultScale(0.15f);
//    tetraObject->Rotate(glm::vec3(1.f, 0.f, 0.f), PI / 2.f);
//    //tetraObject->Rotate(glm::vec3(0.f, 1.f, 0.f), PI / 4.f);
//    tetraObject->CreateAccelerationData(AccelerationTypes::BVH);
//    tetraObject->SetPosition(glm::vec3(0.6f,0.3f,0.43f));
//    newScene->AddSceneObject(tetraObject);

    
    //    std::cout << "Cube scen pos "<<  cubeObjects[0]->get << std:: endl;
    
    //VASE
//    std::vector<std::shared_ptr<MeshObject>> vaseMesh = MeshLoader::LoadMesh("Vase.obj", &loadedMaterials);
//    for (size_t i = 0; i < vaseMesh.size(); ++i) {
//        std::shared_ptr<Material> materialCopy = cubeMaterial->Clone();
//        materialCopy->LoadMaterialFromAssimp(loadedMaterials[i]);
//        materialCopy->SetAmbient(glm::vec3(0.f,0.f,0.f));
//        vaseMesh[i]->SetMaterial(materialCopy);
//    }
//    
//    std::shared_ptr<SceneObject> vase = std::make_shared<SceneObject>();
//    vase->AddMeshObject(vaseMesh);
//    vase->Rotate(glm::vec3(1.f, 0.f, 0.f), PI / 2.f);
//    vase->MultScale(1.7f);
//    vase->SetPosition(glm::vec3(0.2f,0.4f,0.43f));
//    vase->CreateAccelerationData(AccelerationTypes::BVH);
//    newScene->AddSceneObject(vase);
	
    //LIVINGROOM WALLS AND FLOOR
    std::shared_ptr<BlinnPhongMaterial> roomMaterial = std::make_shared<BlinnPhongMaterial>();
    roomMaterial->SetDiffuse(glm::vec3(1.f, 1.f, 1.f));
//    roomMaterial->SetSpecular(glm::vec3(0.6f, 0.6f, 0.6f), 40.f);
    roomMaterial->SetAmbient(glm::vec3(0.2f));
    
    std::vector<std::shared_ptr<aiMaterial>> loadedMaterialsLivingRoom;
    std::vector<std::shared_ptr<MeshObject>>  livingroomTemp = MeshLoader::LoadMesh("dream_room.obj", &loadedMaterialsLivingRoom);
    for (size_t i = 0; i < livingroomTemp.size(); ++i) {
        std::shared_ptr<Material> materialCopy = roomMaterial->Clone();
        materialCopy->LoadMaterialFromAssimp(loadedMaterialsLivingRoom[i]);
        materialCopy->SetAmbient(glm::vec3(0.0f));
        materialCopy->SetSpecular(glm::vec3(0.0f), 0.0f);
        livingroomTemp[i]->SetMaterial(materialCopy);
        
        std::string meshName = livingroomTemp[i]->GetName();
        if(meshName == "Background"){
            materialCopy->SetAffectedByLight(false);
        }
        //Only Transparent Objects
        if(meshName == "Curtain2_curtain2" || meshName == "Curtain1_curtain1"){
            materialCopy->SetTransmittance(0.2f);
        }
        if(meshName == "LampCover_Cylinder.000"){
            materialCopy->SetTransmittance(0.91f);
        }
       	
        //Only Glass Objects
        if(meshName == "Table_glass_top_Glass" || meshName == "window_2" || meshName == "window_1" ){
             std::cout << "Meshname  "<< meshName  << std::endl;
            materialCopy->SetReflectivity(0.435f);
            materialCopy->SetAmbient(glm::vec3(0.00f));
            materialCopy->SetTransmittance(.91f);
            materialCopy->SetIOR(1.44f);
        }
        std::cout << "Meshname  "<< meshName  << std::endl;
        //Reflective metal objects
        if(meshName == "Soffa1_ben" || meshName == "Soffa2_ben" || meshName == "Table_glass_legs_legs" || meshName == "window1"){
            std::cout << "Meshname  "<< meshName  << std::endl;
            materialCopy->SetReflectivity(0.4f);
        }
        //Roof
        if(meshName == "Roof"){
            materialCopy->SetReflectivity(0.04f);
        }
        if(meshName == "Floor"){
            materialCopy->SetReflectivity(0.05f);
        }
        if(meshName == "Whisky" || meshName == "Whisky_in_vase"){
            materialCopy->SetReflectivity(0.1f);
            materialCopy->SetIOR(1.33f);
            materialCopy->SetTransmittance(.41f);
        }
        
        if(meshName == "Whiskey_glass" ||meshName == "Whiskey_glass.001" || meshName == "Icecube"  || meshName == "Icecube.001"  || meshName == "Whiskey_vase" ){
            materialCopy->SetReflectivity(0.1f);
            materialCopy->SetIOR(1.53f);
            materialCopy->SetTransmittance(.91f);
        }
        if(meshName == "Carpet_Plane"){
            materialCopy->SetReflectivity(0.f);
        }
        livingroomTemp[i]->SetMaterial(materialCopy);
        
    }
    std::shared_ptr<SceneObject> livingroomObject = std::make_shared<SceneObject>();
    livingroomObject->AddMeshObject(livingroomTemp);
    livingroomObject->MultScale(1.f);
//    livingroomObject->Rotate(glm::vec3(0.f, 1.f, 0.f), PI / 2.f);
    livingroomObject->CreateAccelerationData(AccelerationTypes::BVH);
    livingroomObject->SetPosition(glm::vec3(0.f, 0.f,0.f));
	newScene->AddSceneObject(livingroomObject);
    
    // Lights
    std::shared_ptr<PointLight> pointLight = std::make_shared<PointLight>();
    //    pointLight->SetPosition(glm::vec3(0.01909f, 0.0101f, 1.97028f));
    pointLight->SetPosition(glm::vec3(-2.4f,-.29f,.3f));
    pointLight->SetPosition(glm::vec3(-0.4f,-.29f,.3f));
    pointLight->SetLightColor(glm::vec3(1.0f, 1.0f, 1.0f)*5.0f);
//    newScene->AddLight(pointLight);
//
//   Right lamp, right to right couch
    std::shared_ptr<PointLight> pointLight2 = std::make_shared<PointLight>();
//        pointLight2->SetPosition(glm::vec3(-5.4f,1.5f, -3.4f));
//    pointLight2->SetPosition(glm::vec3(-0.15f, 4.01f, 4.2328f));
//     pointLight2->SetPosition(glm::vec3(-1.4f,2.f,3.4f));
    pointLight2->SetPosition(glm::vec3(2.25603f,2.29666f,1.9797f));
    pointLight2->SetLightColor(glm::vec3(1.f,1.f,1.f)*1.f);
    newScene->AddLight(pointLight2);
    
//Left lamp, left to left couch
    std::shared_ptr<PointLight> pointLight3 = std::make_shared<PointLight>();
    //    pointLight->SetPosition(glm::vec3(0.01909f, 0.0101f, 1.97028f));
    pointLight3->SetPosition(glm::vec3(-4.65f, 3.0f,-3.78f));
    pointLight3->SetLightColor(glm::vec3(1.0f, 1.0f, 1.0f)*1.f);
    newScene->AddLight(pointLight3);
    
    
    std::shared_ptr<PointLight> pointLight4 = std::make_shared<PointLight>();
    //    pointLight->SetPosition(glm::vec3(0.01909f, 0.0101f, 1.97028f));
    pointLight4->SetPosition(glm::vec3(-4.26099f,1.09292f,3.30543f));
    pointLight4->SetLightColor(glm::vec3(1.0f, 1.0f, 1.0f)*1.f);
    newScene->AddLight(pointLight4);
    
    
    
    
    std::shared_ptr<DirectionalLight> dirLight = std::make_shared<DirectionalLight>();
    //    pointLight->SetPosition(glm::vec3(0.01909f, 0.0101f, 1.97028f));
//    pointLight->SetPosition(glm::vec3(-2.4f,-.29f,.3f));
    dirLight->Rotate(glm::vec3(0.0f, 1.0f,0.0f), -PI);
	dirLight->Rotate(glm::vec3(1.0f, 0.0f,0.0f), PI / 10);
    dirLight->Rotate(glm::vec3(.0f, 1.0f,0.0f), -PI / 3);
    
//        dirLight->SetLightColor(glm::vec3(218.f, 226.f, 214.f)/255.f);
    
    dirLight->SetLightColor(2.f*glm::vec3(182.f,126.f,91.f) / 255.f);
  newScene->AddLight(dirLight);
    
    
//    
//    std::shared_ptr<AreaLight> areaLight = std::make_shared<AreaLight>(glm::vec2(.2f,.2f));
////    //    pointLight->SetPosition(glm::vec3(0.01909f, 0.0101f, 1.97028f));
////    //    pointLight->SetPosition(glm::vec3(-2.4f,-.29f,.3f));
//    areaLight->SetPosition(glm::vec3(1.71f,1.6447f,4.f));
//    areaLight->SetLightColor(glm::vec3(1.0f, 1.0f, 1.0f)*0.6f);
//    newScene->AddLight(areaLight);
//
    return newScene;

}

std::shared_ptr<Scene> Assignment8::LoadCornellScene() const {
    std::shared_ptr<Scene> newScene = std::make_shared<Scene>();
    
    // Material
    std::shared_ptr<BlinnPhongMaterial> cubeMaterial = std::make_shared<BlinnPhongMaterial>();
    cubeMaterial->SetDiffuse(glm::vec3(1.f, 1.f, 1.f));
    cubeMaterial->SetSpecular(glm::vec3(0.6f, 0.6f, 0.6f), 40.f);
//    cubeMaterial->SetReflectivity(0.4f);
    
    // Objects
    std::vector<std::shared_ptr<aiMaterial>> loadedMaterials;
    std::vector<std::shared_ptr<MeshObject>> cubeObjects = MeshLoader::LoadMesh("CornellBox/CornellBox-Assignment8.obj", &loadedMaterials);
    for (size_t i = 0; i < cubeObjects.size(); ++i) {
        std::shared_ptr<Material> materialCopy = cubeMaterial->Clone();
        materialCopy->LoadMaterialFromAssimp(loadedMaterials[i]);
        materialCopy->SetAmbient(glm::vec3(0.f));
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