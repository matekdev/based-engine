#include "game.hpp"

void Game::CalculateDeltaTime()
{
    auto currentFrame = glfwGetTime();
    DeltaTime = currentFrame - _previousFrameTime;
    _previousFrameTime = currentFrame;
}
