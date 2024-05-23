#pragma once

#include "render/frame_buffer.hpp"
#include "render/shader.hpp"
#include "camera/camera.hpp"

#include "component/model/model_component.hpp"
#include "component/transform_component.hpp"

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
    void Resize(float width, float height);
    void Render(GLFWwindow *window);
    float GetDeltaTime();

    Camera &GetCamera();
    GLuint GetRenderTextureId();

private:
    float _previousFrameTime;
    static inline float _deltaTime;

    FrameBuffer _frameBuffer;
    FrameBuffer _pickingBuffer;

    Shader _modelShader;
    Shader _lightShader;
    Shader _outlineShader;

    Camera _camera;
    glm::mat4 _viewProjectionMatrix;

    void CalculateDeltaTime();
    void RenderWithOutline(ModelComponent &model, TransformComponent &transform);
};
