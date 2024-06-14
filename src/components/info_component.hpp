#pragma once

#include <string>
#include <random>

class InfoComponent
{
public:
    std::string Name;
    int Id;

    InfoComponent();

private:
    static inline int _id;
};
