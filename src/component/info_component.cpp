#include "component/info_component.hpp"

InfoComponent::InfoComponent() : Id(InfoComponent::uniformDistribution(InfoComponent::_engine)), Name("Entity") {}

std::random_device InfoComponent::_randomDevice;
std::mt19937_64 InfoComponent::_engine = std::mt19937_64(InfoComponent::_randomDevice());
std::uniform_int_distribution<uint64_t> InfoComponent::uniformDistribution;
