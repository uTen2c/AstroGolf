#pragma once
#include <DxLib.h>

#include "KeyInstance.h"

class Device
{
    int last_mouse_input_ = 0;
    int current_mouse_input_ = 0;

public:
    KeyInstance debugKey = KeyInstance(KEY_INPUT_F3);

    Device();

    void Update();

    [[nodiscard]] bool LeftClicked() const;
    [[nodiscard]] bool LeftClicking() const;
    [[nodiscard]] bool RightClicked() const;
    [[nodiscard]] bool RightClicking() const;
};
