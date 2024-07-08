#include "components/info_component.hpp"

InfoComponent::InfoComponent(const entt::entity &entity) : Component(entity), Name("Entity") {}

std::uint32_t InfoComponent::GetId() const
{
    return static_cast<std::uint32_t>(_entity);
}
