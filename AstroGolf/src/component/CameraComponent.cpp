#include "CameraComponent.h"

#include <DxLib.h>
#include <spdlog/spdlog.h>

void CameraComponent::Update(float delta)
{
    int x;
    int y;

    GetMousePoint(&x, &y);

    const auto clicking = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;
    if (!dragging_ && clicking)
    {
        dragging_ = true;
        prev_pos = transform.translate;
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

    const auto dx = drag_start_pos_x_ - x;
    const auto dy = drag_start_pos_y_ - y;
    const auto newPos = Vec2(prev_pos.x + dx, prev_pos.y + dy);
    transform.translate = newPos;
}
