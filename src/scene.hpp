#pragma once

#include "camera/camera.hpp"
#include "components/model/model_component.hpp"
#include "render/vertex_shader.hpp"
#include "render/pixel_shader.hpp"
#include "render/render_target.hpp"
#include "render/skybox.hpp"

#include <PxPhysicsAPI.h>

#include <optional>

class Scene
{
public:
    Scene();
    ~Scene();

    // Ideally this shouldn't be static and public... but I'm here to do graphics shit.
    static inline Scene *ActiveScene;
    std::optional<entt::entity> SelectedEntity;
    entt::registry Registry;

    ID3D11ShaderResourceView *GetShaderResourceView();
    physx::PxScene *GetPhysicsScene();
    physx::PxPhysics *GetPhysics();

    Camera &GetActiveCamera();
    float GetDeltaTime();

    entt::entity CreateNewEntity();
    void OnResize();
    void Render();

private:
    const std::string LOCAL_HOST = "127.0.0.1";

    Camera _camera;

    VertexShader _modelVertexShader;
    PixelShader _modelPixelShader;

    SkyBox _skybox;

    RenderTarget _renderTarget;

    physx::PxDefaultAllocator _pxAllocator;
    physx::PxDefaultErrorCallback _pxErrorCallback;
    physx::PxFoundation *_pxFoundation;
    physx::PxPhysics *_pxPhysics;
    physx::PxDefaultCpuDispatcher *_pxDispatcher;
    physx::PxScene *_pxScene;
    physx::PxPvd *_pxPvd;

    float _previousFrameTime;
    float _deltaTime;

    void RenderModels();
    void RenderSkyBox();
    void RenderLight();

    void CalculateDeltaTime();
    void InitializePhysics();
    void InitializeDefaultScene();
};
