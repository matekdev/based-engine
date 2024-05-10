#include "scene.hpp"

void Scene::CalculateDeltaTime()
{
    auto currentFrame = glfwGetTime();
    DeltaTime = currentFrame - _previousFrameTime;
    _previousFrameTime = currentFrame;
}
