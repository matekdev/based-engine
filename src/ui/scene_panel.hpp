#pragma once

#include "panel.hpp"

class ScenePanel : public Panel
{
public:
    static bool IsHovered();
    static float GetWidth();
    static float GetHeight();

    void Draw() override;

private:
    static inline bool _isHovered;
    static inline float _width = 1.0f;
    static inline float _height = 1.0f;
};
