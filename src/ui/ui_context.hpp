#pragma once

class Window;

class UIContext
{
public:
    UIContext(Window *window);

    void PreRender();
    void PostRender();
    void Destroy();
};