#pragma once
#include <imgui.h>
#include <string>
#include <vector>

class ImEx
{
public:
    static bool Combo(const std::string& caption, std::string& currentItem, const std::vector<std::string>& items);
    static bool InputText(const char* label, std::string* str, ImGuiInputTextFlags flags = 0, int bufferSize = 1024);
};
