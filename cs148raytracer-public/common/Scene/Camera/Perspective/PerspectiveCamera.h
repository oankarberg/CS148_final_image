#pragma once

#include "common/Scene/Camera/Camera.h"

class PerspectiveCamera : public Camera
{
public:
    // inputFov is in degrees. 
    PerspectiveCamera(float aspectRatio, float inputFov);
    virtual std::shared_ptr<class Ray> GenerateRayForNormalizedCoordinates(glm::vec2 coordinate) const override;
    virtual std::shared_ptr<class Ray> GenerateDOFRayForNormalizedCoordinates(glm::vec2 coordinate, float apertureR) const override;

    void SetZNear(float input);
    void SetZFar(float input);

private:
    float aspectRatio;
    float fov; // fov is stored as radians

    float zNear;
    float zFar;
};