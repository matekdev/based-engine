#pragma once

#include "component.hpp"

#include <string>
#include <random>

class InfoComponent : public Component
{
public:
    std::string Name;

    InfoComponent(const entt::entity &entity);

    unsigned int GetId() const;
};
