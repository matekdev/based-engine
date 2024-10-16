#include "scene.hpp"

#include "components/info_component.hpp"
#include "components/transform_component.hpp"
#include "components/ignore_component.hpp"
#include "components/directional_light_component.hpp"
#include "components/model/model_component.hpp"

#include <GLFW/glfw3.h>

Scene::Scene() : _renderTarget(RenderTarget()),
                 _camera(Camera()),
                 _skybox(SkyBox()),
                 _modelVertexShader(L"shaders/model.vs.hlsl"),
                 _modelPixelShader(L"shaders/model.ps.hlsl")
{
    ActiveScene = this;

    InitializePhysics();
    InitializeDefaultScene();
}

Scene::~Scene()
{
    PX_RELEASE(_pxScene);
    PX_RELEASE(_pxDispatcher);
    PX_RELEASE(_pxPhysics);
    if (_pxPvd)
    {
        auto *transport = _pxPvd->getTransport();
        PX_RELEASE(_pxPvd);
        PX_RELEASE(transport);
    }

    PX_RELEASE(_pxFoundation);
}

ID3D11ShaderResourceView *Scene::GetShaderResourceView()
{
    return _renderTarget.GetShaderResourceView();
}

physx::PxScene *Scene::GetPhysicsScene()
{
    return _pxScene;
}

physx::PxPhysics *Scene::GetPhysics()
{
    return _pxPhysics;
}

Camera &Scene::GetActiveCamera()
{
    return _camera;
}

float Scene::GetDeltaTime()
{
    return _deltaTime;
}

entt::entity Scene::CreateNewEntity()
{
    auto ent = Registry.create();
    Registry.emplace<InfoComponent>(ent, ent);
    Registry.emplace<TransformComponent>(ent, ent);
    return ent;
}

void Scene::OnResize()
{
    _renderTarget.Resize();
}

void Scene::Render()
{
    _camera.Update();
    CalculateDeltaTime();

    _renderTarget.Bind();

    RenderModels();
    RenderSkyBox();
    RenderLight();

    _pxScene->simulate(1.0f / 60.0f);
    _pxScene->fetchResults(true);
}

void Scene::RenderModels()
{
    _renderTarget.SetMode(RenderTarget::Mode::Default);

    const auto modelGroup = Scene::ActiveScene->Registry.view<ModelComponent, TransformComponent>(entt::exclude<IgnoreComponent>);
    for (const auto &entity : modelGroup)
    {
        auto &transform = modelGroup.get<TransformComponent>(entity);
        const auto &model = modelGroup.get<ModelComponent>(entity);

        _modelVertexShader.Bind();
        _modelPixelShader.Bind();
        transform.Bind();

        model.Render();
    }
}

void Scene::RenderSkyBox()
{
    _renderTarget.SetMode(RenderTarget::Mode::DepthFirst);
    _skybox.Render();
}

void Scene::RenderLight()
{
    const auto lightGroup = Scene::ActiveScene->Registry.view<TransformComponent, DirectionalLightComponent>(entt::exclude<IgnoreComponent>);
    for (const auto &entity : lightGroup)
    {
        auto &transform = lightGroup.get<TransformComponent>(entity);
        auto &directionalLight = lightGroup.get<DirectionalLightComponent>(entity);

        _modelVertexShader.Bind();
        _modelPixelShader.Bind();

        transform.Bind();
        directionalLight.Bind();
    }
}

void Scene::CalculateDeltaTime()
{
    auto currentFrame = glfwGetTime();
    _deltaTime = currentFrame - _previousFrameTime;
    _previousFrameTime = currentFrame;
}

void Scene::InitializePhysics()
{
    _pxFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, _pxAllocator, _pxErrorCallback);

    _pxPvd = physx::PxCreatePvd(*_pxFoundation);
    auto *transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
    _pxPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

    _pxPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *_pxFoundation, physx::PxTolerancesScale(1.0f, 10.0f), true, _pxPvd);

    auto sceneDesc = physx::PxSceneDesc(_pxPhysics->getTolerancesScale());
    sceneDesc.gravity = physx::PxVec3(0.0f, -5.0f, 0.0f); // TODO: Maybe some way to change this.
    _pxDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = _pxDispatcher;
    sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
    _pxScene = _pxPhysics->createScene(sceneDesc);

    auto *pvdClient = _pxScene->getScenePvdClient();
    if (pvdClient)
    {
        pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
        pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
        pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
    }
}

void Scene::InitializeDefaultScene()
{
    // Floor
    auto floor = CreateNewEntity();
    Registry.get<InfoComponent>(floor).Name = "Floor";
    Registry.get<TransformComponent>(floor).SetPosition(glm::vec3(0.0f, -1.0f, 0.0f));
    Registry.emplace<ModelComponent>(floor, floor).LoadModel("models\\plane\\plane.obj");

    // Sun
    auto sun = CreateNewEntity();
    Registry.get<InfoComponent>(sun).Name = "Sun";
    Registry.get<TransformComponent>(sun).SetPosition(glm::vec3(-10.0f, 0.0f, 0.0f));
    Registry.get<TransformComponent>(sun).SetRotation(glm::vec3(2.0f, 0.0f, 0.4f));
    Registry.emplace<DirectionalLightComponent>(sun, sun);
}
