#pragma once

class Window;

class OpenGLContext
{
public:
    OpenGLContext(Window *window);
    void PreRender();
    void PostRender();
    void Destroy();

private:
    Window *_window;
};