#include "Device.h"

#include <DxLib.h>

Device::Device()
{
    current_mouse_input_ = GetMouseInput();
    last_mouse_input_ = current_mouse_input_;
}

void Device::Update()
{
    last_mouse_input_ = current_mouse_input_;
    current_mouse_input_ = GetMouseInput();

    toggleUiKey.Update();
    debugKey.Update();
    menuKey.Update();
    ctrlKey.Update();
    deleteKey.Update();
}

bool Device::LeftClicked()
{
    const auto clicked = (last_mouse_input_ & MOUSE_INPUT_LEFT) == 0
        && (current_mouse_input_ & MOUSE_INPUT_LEFT) != 0;
    if (clicked)
    {
        left_clicked_pos_ = MousePos();
    }
    return clicked;
}

bool Device::LeftReleased(const bool allowMove) const
{
    const auto released = (last_mouse_input_ & MOUSE_INPUT_LEFT) != 0
        && (current_mouse_input_ & MOUSE_INPUT_LEFT) == 0;
    if (allowMove)
    {
        return released;
    }
    return left_clicked_pos_.Distance(MousePos()) < 10;
}

bool Device::LeftClicking() const
{
    return (current_mouse_input_ & MOUSE_INPUT_LEFT) != 0;
}

bool Device::RightClicked() const
{
    return (last_mouse_input_ & MOUSE_INPUT_RIGHT) == 0
        && (current_mouse_input_ & MOUSE_INPUT_RIGHT) != 0;
}

bool Device::RightClicking() const
{
    return (current_mouse_input_ & MOUSE_INPUT_RIGHT) != 0;
}

Vec2 Device::MousePos() const
{
    int x, y;
    GetMousePoint(&x, &y);
    return {static_cast<float>(x), static_cast<float>(y)};
}
