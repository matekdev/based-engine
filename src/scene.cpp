#include "scene.hpp"

#include "components/info_component.hpp"
#include "components/transform_component.hpp"
#include "components/model/model_component.hpp"

#include <GLFW/glfw3.h>

Scene::Scene() : _renderTarget(RenderTarget()),
                 _camera(Camera()),
                 _modelVertexShader(L"shaders/model.vs.hlsl"),
                 _modelPixelShader(L"shaders/model.ps.hlsl")
{
    ActiveScene = this;

    InitializePhysics();

    // TODO: Remove debug.
    auto ent = Registry.create();
    Registry.emplace<InfoComponent>(ent, ent);
    Registry.emplace<TransformComponent>(ent, ent);
    auto &model = Registry.emplace<ModelComponent>(ent, ent);
    model.LoadModel("models\\dev_orange_cube\\dev_orange_cube.obj");
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

Camera &Scene::GetActiveCamera()
{
    return _camera;
}

float Scene::GetDeltaTime()
{
    return _deltaTime;
}

void Scene::CreateNewEntity()
{
    auto ent = Registry.create();
    Registry.emplace<InfoComponent>(ent, ent);
    Registry.emplace<TransformComponent>(ent, ent);
    Registry.emplace<ModelComponent>(ent, ent);
}

void Scene::OnResize()
{
    _renderTarget.Resize();
}

void Scene::Render()
{
    CalculateDeltaTime();

    _renderTarget.Bind();

    const auto modelGroup = Scene::ActiveScene->Registry.view<ModelComponent, TransformComponent>();
    for (const auto &entity : modelGroup)
    {
        auto &transform = modelGroup.get<TransformComponent>(entity);
        const auto &model = modelGroup.get<ModelComponent>(entity);

        _modelVertexShader.Bind();
        _modelPixelShader.Bind();
        transform.Bind();

        model.Render();
    }

    _camera.Update();

    _pxScene->simulate(1.0f / 60.0f);
    _pxScene->fetchResults(true);
}

void Scene::CalculateDeltaTime()
{
    auto currentFrame = glfwGetTime();
    _deltaTime = currentFrame - _previousFrameTime;
    _previousFrameTime = currentFrame;
}

// TODO:: remove debug method
void Scene::createStack(const physx::PxTransform &t, physx::PxU32 size, physx::PxReal halfExtent)
{
    auto *shape = _pxPhysics->createShape(physx::PxBoxGeometry(halfExtent, halfExtent, halfExtent), *_pxMaterial);
    for (physx::PxU32 i = 0; i < size; i++)
    {
        for (physx::PxU32 j = 0; j < size - i; j++)
        {
            physx::PxTransform localTm(physx::PxVec3(physx::PxReal(j * 2) - physx::PxReal(size - i), physx::PxReal(i * 2 + 1), 0) * halfExtent);
            physx::PxRigidDynamic *body = _pxPhysics->createRigidDynamic(t.transform(localTm));
            body->attachShape(*shape);
            physx::PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
            _pxScene->addActor(*body);
        }
    }
    shape->release();
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

    // TODO: Remove this, we should create the ground plane somewhere else.
    _pxMaterial = _pxPhysics->createMaterial(0.5f, 0.5f, 0.6f);

    auto *groundPlane = physx::PxCreatePlane(*_pxPhysics, physx::PxPlane(0, 1, 0, 0), *_pxMaterial);
    _pxScene->addActor(*groundPlane);

    for (physx::PxU32 i = 0; i < 5; i++)
        createStack(physx::PxTransform(physx::PxVec3(0, 0, 0)), 10, 2.0f);
}
