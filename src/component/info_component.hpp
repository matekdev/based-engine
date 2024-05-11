#pragma once

#include <string>

class InfoComponent
{
public:
    std::string Name = "test";
    int Id = _id++;

private:
    // Someone else can implement a more sophisticated id system...
    static inline int _id = 0;
};
