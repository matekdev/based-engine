#pragma once

#include "ui/scene_panel.hpp"

#include <entt/entt.hpp>
#include <GLFW/glfw3.h>

#include <unordered_map>
#include <vector>

class GameObject;

class Scene
{
public:
    Scene();

    static inline Scene *ActiveScene;

    void CreateGameObject();

    void Render(GLFWwindow *window);
    float GetDeltaTime();

private:
    friend class GameObject;

    ScenePanel _scenePanel;
    entt::registry _registry;

    float _previousFrameTime;
    static inline float _deltaTime;

    void CalculateDeltaTime();
};
