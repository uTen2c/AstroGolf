#include "ImEx.h"

#include <imgui.h>
#include <cstring>

bool ImEx::Combo(const std::string& caption, std::string& currentItem, const std::vector<std::string>& items)
{
    bool changed = false;

    if (ImGui::BeginCombo(caption.c_str(), currentItem.c_str()))
    {
        for (const auto& item : items)
        {
            const bool isSelected = (currentItem == item);
            if (ImGui::Selectable(item.c_str(), isSelected))
            {
                currentItem = item;
                changed = true;
            }
            if (isSelected)
            {
                // Set the initial focus when opening the combo (scrolling + for
                // keyboard navigation support in the upcoming navigation branch)
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    return changed;
}

bool ImEx::InputText(const char* label, std::string* str, const ImGuiInputTextFlags flags, const int bufferSize)
{
    // 一時的なバッファを作成
    const auto buffer = new char[bufferSize];

    // stringの内容をバッファにコピー
    strncpy_s(buffer, bufferSize, str->c_str(), bufferSize - 1);
    buffer[bufferSize - 1] = '\0';

    // ImGui::InputTextを実行
    const bool changed = ImGui::InputText(label, buffer, bufferSize, flags);

    // 変更があった場合、stringに反映
    if (changed)
    {
        *str = buffer;
    }

    // バッファを解放
    delete[] buffer;

    return changed;
}
