#pragma once
#include <string>
#include <vector>

class ImEx
{
public:
    static bool Combo(const std::string& caption, std::string& currentItem, const std::vector<std::string>& items);
};
