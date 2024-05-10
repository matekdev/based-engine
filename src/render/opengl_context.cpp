#include "opengl_context.hpp"

#include "window.hpp"

OpenGLContext::OpenGLContext(Window *window) : _window(window)
{
}

void OpenGLContext::PreRender()
{
}

void OpenGLContext::PostRender()
{
    glfwSwapBuffers(_window->GetNativeWindow());
    glfwPollEvents();
}

void OpenGLContext::Destroy()
{
    glfwDestroyWindow(_window->GetNativeWindow());
    glfwTerminate();
}