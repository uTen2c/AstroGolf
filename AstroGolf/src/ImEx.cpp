#include "ImEx.h"

#include <imgui.h>

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
