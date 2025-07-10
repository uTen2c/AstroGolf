#pragma once
#include <DxLib.h>

#include "KeyInstance.h"
#include "math/Vec2.h"

class Device
{
    int last_mouse_input_ = 0;
    int current_mouse_input_ = 0;
    Vec2 left_clicked_pos_;
    Vec2 right_clicked_pos_;

public:
    KeyInstance toggleUiKey = KeyInstance(KEY_INPUT_F1);
    KeyInstance debugKey = KeyInstance(KEY_INPUT_F3);
    KeyInstance menuKey = KeyInstance(KEY_INPUT_ESCAPE);
    KeyInstance ctrlKey = KeyInstance(KEY_INPUT_LCONTROL);
    KeyInstance deleteKey = KeyInstance(KEY_INPUT_DELETE);

    Device();

    void Update();

    [[nodiscard]] bool LeftClicked();
    [[nodiscard]] bool LeftReleased(bool allowMove = true) const;
    [[nodiscard]] bool LeftClicking() const;
    [[nodiscard]] bool RightClicked();
    [[nodiscard]] bool RightReleased(bool allowMove = true) const;
    [[nodiscard]] bool RightClicking() const;
    [[nodiscard]] Vec2 MousePos() const;
};
