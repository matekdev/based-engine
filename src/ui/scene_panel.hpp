#pragma once

#include "render/frame_buffer.hpp"
#include "render/shader.hpp"
#include "camera/camera.hpp"
#include "ui/components/gizmo.hpp"

#include <vector>

class ScenePanel
{
public:
    ScenePanel();

    void Render(GLFWwindow *window);
    void Input(GLFWwindow *window);
    void Resize(float width, float height);
    void OnMouseClick();

private:
    glm::vec2 _viewPortBounds[2];
    float _width;
    float _height;

    FrameBuffer _pickingBuffer;
    Shader _pickingShader;

    ImGuizmo::OPERATION _activeGizmo = ImGuizmo::OPERATION::TRANSLATE;

    void HandleGizmo(GLFWwindow *window);
    void PickingPass();
};