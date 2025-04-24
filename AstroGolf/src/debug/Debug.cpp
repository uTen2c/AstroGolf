#include "Debug.h"

#include <imgui.h>

void Debug::DrawDebugMenu()
{
    ImGui::Begin("Debug menu");

    ImGui::Checkbox("Show collisions", &show_collisions);
    
    ImGui::End();
}
