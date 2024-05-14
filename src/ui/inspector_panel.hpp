#pragma once

#include <functional>
#include <string>

class InspectorPanel
{
public:
    InspectorPanel();

    void Render();

private:
    template <typename T>
    void ComponentHeader(const std::string &name, const std::function<void()> &options);

    template <typename T>
    void RemoveComponentButton();

    template <typename T>
    void AddComponentEntry(const std::string &name);
};
