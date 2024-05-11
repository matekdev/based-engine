#pragma once

#include <string>

class InspectorPanel
{
public:
    InspectorPanel();

    void Render();

private:
    template <typename T>
    void DisplayAddComponentEntry(const std::string &name);
};
