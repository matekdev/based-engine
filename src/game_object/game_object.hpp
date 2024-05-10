#pragma once

#include "scene.hpp"
#include <entt/entt.hpp>

class GameObject
{
public:
    GameObject(entt::entity handle, Scene *scene);

    template <typename T, typename... Args>
    T &AddComponent(Args &&...args)
    {
        T &component = _scene->_registry.emplace<T>(_entityHandle, std::forward<Args>(args)...);
        return component;
    }

    template <typename T>
    T &GetComponent()
    {
        return _scene->_registry.get<T>(_entityHandle);
    }

    template <typename T>
    bool HasComponent()
    {
        return _scene->_registry.any_of<T>(_entityHandle);
    }

    template <typename T>
    void RemoveComponent()
    {
        _scene->_registry.remove<T>(_entityHandle);
    }

private:
    entt::entity _entityHandle{entt::null};
    Scene *_scene;
};
