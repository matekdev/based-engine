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

    FrameBuffer _frameBuffer;
    FrameBuffer _pickingBuffer;

    Shader _modelShader;
    Shader _pickingShader;

    Camera _camera;
    glm::mat4 _viewProjectionMatrix;
    ImGuizmo::OPERATION _activeGizmo = ImGuizmo::OPERATION::TRANSLATE;

    void RenderPass();
    void HandleGizmo(GLFWwindow *window);
    void PickingPass();
};