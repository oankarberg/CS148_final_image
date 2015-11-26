#pragma once

#include "common/Scene/SceneObject.h"

class Camera : public SceneObject
{
public:
    Camera();

    virtual std::shared_ptr<class Ray> GenerateRayForNormalizedCoordinates(glm::vec2 coordinate) const = 0;
    virtual std::shared_ptr<class Ray> GenerateDOFRayForNormalizedCoordinates(glm::vec2 coordinate, float apertureR) const = 0;
};