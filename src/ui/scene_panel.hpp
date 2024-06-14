#pragma once

#include "panel.hpp"

#include "components/gizmo.hpp"

#include <glm/glm.hpp>

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

    ImGuizmo::OPERATION _activeGizmo = ImGuizmo::OPERATION::TRANSLATE;
    glm::vec2 _viewPortBounds[2];

    void UpdateGizmo();
};
