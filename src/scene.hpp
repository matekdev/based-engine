#pragma once

#include "ui/scene_panel.hpp"
#include "game_object/game_object.hpp"

#include <GLFW/glfw3.h>
#include <vector>

#include <entt/entt.hpp>

class Scene
{
public:
    Scene();

    void Render(GLFWwindow *window);
    static float GetDeltaTime();

private:
    ScenePanel _scenePanel;
    entt::registry _registry;

    float _previousFrameTime;
    static inline float _deltaTime;

    void CalculateDeltaTime();
};
