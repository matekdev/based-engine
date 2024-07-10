#pragma once

#include "panel.hpp"

#include <functional>
#include <iostream>

class InspectorPanel : public Panel
{
public:
    InspectorPanel();

    void Draw() override;

private:
    std::vector<std::string> _modelPaths;

    void ComponentEntries();

    template <typename T>
    void ComponentHeader(const std::string &name, const std::function<void(T *)> &options, const bool &canDelete = true);

    template <typename T>
    void ComponentEntry(const std::string &name);
};
