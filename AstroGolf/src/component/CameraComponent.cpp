﻿#include "CameraComponent.h"

#include <DxLib.h>
#include <imgui.h>
#include <spdlog/spdlog.h>

#include "../Game.h"
#include "../world/World.h"

void CameraComponent::Update(float delta)
{
    UpdatePos();
    UpdateZoom();

    if (Game::debugEnabled && world->GetType() != WorldType::StageSelect)
    {
        ImGui::Begin("Camera", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::InputFloat("X", &transform.translate.x, 0.1f, 1);
        ImGui::InputFloat("Y", &transform.translate.y, 0.1f, 1);

        ImGui::Text("Zoom: %0.3f", zoom);

        if (ImGui::Button("Reset pos"))
        {
            transform.translate = {0, 0};
        }

        ImGui::End();
    }
}

void CameraComponent::UpdatePos()
{
    if (Game::instance->isPaused || !draggable)
    {
        return;
    }
    
    int x;
    int y;

    GetMousePoint(&x, &y);

    const auto playerDragging = world->GetPlayer()->isDragging;
    const auto& io = ImGui::GetIO();

    const auto clicking = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;
    if (!dragging_ && clicking && !playerDragging && !io.WantCaptureMouse)
    {
        dragging_ = true;
        prev_pos_ = transform.translate;
        drag_start_pos_x_ = x;
        drag_start_pos_y_ = y;
    }

    if (dragging_ && (!clicking || playerDragging))
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
    world->OnCameraMoveWithMouse(this);
}

void CameraComponent::UpdateZoom()
{
    if (!world->zoomEnabled)
    {
        return;
    }
    const auto rot = GetMouseWheelRotVol();
    zoom = std::clamp(zoom + static_cast<float>(rot) * 0.1f, minZoomLevel, maxZoomLevel);
}
