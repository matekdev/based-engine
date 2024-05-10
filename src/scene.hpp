#pragma once

#include "game_object/game_object.hpp"

#include <GLFW/glfw3.h>
#include <vector>

class Scene
{
public:
    static inline std::vector<GameObject> GameObjects = {};
    static inline GameObject *SelectedGameObject;
    static inline float DeltaTime;

    static void CalculateDeltaTime();

private:
    static inline float _previousFrameTime;
};