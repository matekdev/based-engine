#pragma once

#include <vector>
#include <string>

class ObjectsPanel
{
public:
    ObjectsPanel();

    void Render();

private:
    std::vector<std::string> _modelPaths;
};