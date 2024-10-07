#include "scene.hpp"

#include "components/info_component.hpp"
#include "components/transform_component.hpp"
#include "components/skybox_component.hpp"
#include "components/ignore_component.hpp"
#include "components/model/model_component.hpp"

#include <GLFW/glfw3.h>

Scene::Scene() : _renderTarget(RenderTarget()),
                 _camera(Camera()),
                 _modelVertexShader(L"shaders/model.vs.hlsl"),
                 _modelPixelShader(L"shaders/model.ps.hlsl"),
                 _skyboxVertexShader(L"shaders/skybox.vs.hlsl"),
                 _skyboxPixelShader(L"shaders/skybox.ps.hlsl")
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
    CalculateDeltaTime();

    _renderTarget.Bind();

    RenderModels();
    RenderSkyBox();

    _camera.Update();
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

    const auto skyboxGroup = Scene::ActiveScene->Registry.view<SkyBoxComponent, ModelComponent>();
    for (const auto &entity : skyboxGroup)
    {
        const auto &model = skyboxGroup.get<ModelComponent>(entity);

        _skyboxVertexShader.Bind();
        _skyboxPixelShader.Bind();

        model.Render();
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
    sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f); // TODO: Maybe some way to change this.
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

    // Skybox
    auto skybox = CreateNewEntity();
    Registry.get<InfoComponent>(skybox).Name = "SkyBox";
    Registry.get<TransformComponent>(skybox).SetScale(glm::vec3(0.0f));
    Registry.emplace<IgnoreComponent>(skybox);
    Registry.emplace<SkyBoxComponent>(skybox, skybox);
    Registry.emplace<ModelComponent>(skybox, skybox).LoadModel("models\\ignored\\cubemap\\cubemap.obj");
}
