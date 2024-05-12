#pragma once

#include <string>

class InspectorPanel
{
public:
    InspectorPanel();

    void Render();

private:
    template <typename T>
    void RemoveComponentButton();

    template <typename T>
    void AddComponentEntry(const std::string &name);
};
