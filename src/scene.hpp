#pragma once

#include "camera/camera.hpp"
#include "component/model/model_component.hpp"

class Scene
{
public:
    Scene();

    // Ideally this shouldn't be static and public... but I'm here to do graphics shit.
    static inline Scene *ActiveScene;

    float GetDeltaTime();
    Camera &GetCamera();

    void Render();

private:
    Camera _camera;
    ModelComponent _model;

    float _previousFrameTime;
    float _deltaTime;

    void CalculateDeltaTime();
};
