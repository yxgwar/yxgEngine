#pragma once

#include <unordered_map>

class Input
{
public:
    inline static bool GetKey(int key) {return keyStates[key];}
    inline static std::unordered_map<int, bool> keyStates;
private:
    Input() = delete;
};