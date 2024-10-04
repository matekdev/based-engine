#include "components/info_component.hpp"

InfoComponent::InfoComponent(const entt::entity &entity) : Component(entity), Name("Entity") {}

int InfoComponent::GetId() const
{
    return static_cast<int>(_entity);
}
