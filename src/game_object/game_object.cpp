#include "game_object.hpp"

GameObject::GameObject(entt::entity handle, Scene *scene) : _entityHandle(handle), _scene(scene)
{
}
