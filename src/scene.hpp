#pragma once

#include "game_object/game_object.hpp"

#include <GLFW/glfw3.h>
#include <vector>

class Scene
{
public:
    // Ideally none of these are static but the real world is sometimes messy.
    // (I'm too lazy and there will only ever be one scene...)
    static inline std::vector<GameObject> GameObjects = {};
    static inline GameObject *SelectedGameObject;
    static inline float DeltaTime;

    static void CalculateDeltaTime();

private:
    static inline float _previousFrameTime;
};