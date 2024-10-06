#pragma once

#include <entt/entt.hpp>

class Component
{
public:
    Component(const entt::entity &entity);
    Component(const Component &) = delete;
    Component &operator=(const Component &) = delete;

protected:
    entt::entity _entity;
};
