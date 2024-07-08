#pragma once

#include <entt/entt.hpp>

class Component
{
public:
    Component(const entt::entity &entity);

protected:
    entt::entity _entity;
};
