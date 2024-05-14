#pragma once

#include <functional>
#include <string>
#include <vector>

class InspectorPanel
{
public:
    InspectorPanel();

    void Render();

private:
    template <typename T>
    void ComponentHeader(const std::string &name, const std::function<void(T *)> &options);

    template <typename T>
    void AddComponentEntry(const std::string &name);

    std::vector<std::string> _modelPaths;
};
