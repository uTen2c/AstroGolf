#include "CameraComponent.h"

#include <DxLib.h>
#include <imgui.h>
#include <spdlog/spdlog.h>

void CameraComponent::Update(float delta)
{
    UpdatePos();
    UpdateZoom();

    ImGui::Begin("Camera", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    
    ImGui::Text("Zoom: %0.3f", zoom);
    
    if (ImGui::Button("Reset pos"))
    {
        transform.translate = {0, 0};
    }
    
    ImGui::End();
}

void CameraComponent::UpdatePos()
{
    int x;
    int y;

    GetMousePoint(&x, &y);
    
    const auto clicking = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;
    if (!dragging_ && clicking)
    {
        dragging_ = true;
        prev_pos_ = transform.translate;
        drag_start_pos_x_ = x;
        drag_start_pos_y_ = y;
    }

    if (dragging_ && !clicking)
    {
        dragging_ = false;
    }

    if (!dragging_)
    {
        return;
    }

    auto dx = static_cast<float>(drag_start_pos_x_) - static_cast<float>(x);
    auto dy = static_cast<float>(drag_start_pos_y_) - static_cast<float>(y);

    dx /= zoom;
    dy /= zoom;
    
    const auto newPos = Vec2(prev_pos_.x + dx, prev_pos_.y + dy);
    transform.translate = newPos;
}

void CameraComponent::UpdateZoom()
{
    const auto rot = GetMouseWheelRotVol();
    zoom = max(zoom + static_cast<float>(rot) * 0.1f, 0.1f);
}
