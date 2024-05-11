#pragma once

#include <string>
#include <random>

class InfoComponent
{
public:
    std::string Name;
    uint64_t Id;

    InfoComponent();

private:
    static std::random_device _randomDevice;
    static std::mt19937_64 _engine;
    static std::uniform_int_distribution<uint64_t> uniformDistribution;
};
