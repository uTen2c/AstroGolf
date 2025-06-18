#include "KeyInstance.h"

#include <DxLib.h>

void KeyInstance::Update()
{
    if (CheckHitKey(key_))
    {
        if (pressed_)
        {
            pressed_ = false;
        }
        if (!pressing_)
        {
            pressed_ = true;
        }
        pressing_ = true;
    } else
    {
        if (pressing_)
        {
            pressing_ = false;
        }
    }
}

bool KeyInstance::Pressing() const
{
    return pressing_;
}

bool KeyInstance::Pressed() const
{
    return pressed_;
}
