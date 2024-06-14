#pragma once

#include "panel.hpp"

class ScenePanel : public Panel
{
public:
    static const float &GetWidth();
    static const float &GetHeight();

    void Draw() override;

private:
    static inline float _width = 1.0f;
    static inline float _height = 1.0f;
};
