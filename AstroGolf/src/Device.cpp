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

    debugKey.Update();
    menuKey.Update();
    ctrlKey.Update();
    deleteKey.Update();
}

bool Device::LeftClicked() const
{
    return (last_mouse_input_ & MOUSE_INPUT_LEFT) == 0
        && (current_mouse_input_ & MOUSE_INPUT_LEFT) != 0;
}

bool Device::LeftReleased() const
{
    return (last_mouse_input_ & MOUSE_INPUT_LEFT) != 0
    && (current_mouse_input_ & MOUSE_INPUT_LEFT) == 0;
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
