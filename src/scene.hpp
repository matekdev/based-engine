#pragma once

#include "camera/camera.hpp"
#include "component/model/model_component.hpp"
#include "render/vertex_shader.hpp"
#include "render/pixel_shader.hpp"

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

    VertexShader _vertexShader;
    PixelShader _pixelShader;

    float _previousFrameTime;
    float _deltaTime;

    void CalculateDeltaTime();
};
