#pragma once

#include "ui/scene_panel.hpp"

#include <entt/entt.hpp>
#include <GLFW/glfw3.h>

#include <unordered_map>
#include <vector>
#include <optional>

class Scene
{
public:
    Scene();

    // Ideally this shouldn't be static and public... but I'm here to do graphics shit.
    static inline Scene *ActiveScene;
    std::optional<entt::entity> SelectedEntity;
    entt::registry Registry;

    void CreateNewEntity();
    void Render(GLFWwindow *window);
    float GetDeltaTime();
    void OnMouseClick();

private:
    ScenePanel _scenePanel;

    float _previousFrameTime;
    static inline float _deltaTime;

    void CalculateDeltaTime();
};
