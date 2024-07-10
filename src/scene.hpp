#pragma once

#include "camera/camera.hpp"
#include "components/model/model_component.hpp"
#include "render/vertex_shader.hpp"
#include "render/pixel_shader.hpp"
#include "render/render_target.hpp"

#include <optional>

class Scene
{
public:
    Scene();

    // Ideally this shouldn't be static and public... but I'm here to do graphics shit.
    static inline Scene *ActiveScene;
    std::optional<entt::entity> SelectedEntity;
    entt::registry Registry;

    ID3D11ShaderResourceView *GetShaderResourceView();
    Camera &GetActiveCamera();
    float GetDeltaTime();

    void CreateNewEntity();
    void OnResize();
    void Render();

private:
    Camera _camera;

    VertexShader _modelVertexShader;
    PixelShader _modelPixelShader;

    RenderTarget _renderTarget;

    float _previousFrameTime;
    float _deltaTime;

    void CalculateDeltaTime();
};
