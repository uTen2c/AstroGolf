#pragma once

class Device
{
    int last_mouse_input_ = 0;
    int current_mouse_input_ = 0;

public:
    Device();

    void Update();

    bool LeftClicked() const;
    bool LeftClicking() const;
    bool RightClicked() const;
    bool RightClicking() const;
};
