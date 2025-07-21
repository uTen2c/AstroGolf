#pragma once
#include <memory>

#include "Sound.h"

class Sounds
{
    Sounds() = default;

public:
    inline static std::unique_ptr<Sound> uiButtonHover;
    inline static std::unique_ptr<Sound> uiButtonClick;

    static void Load()
    {
        uiButtonHover = std::make_unique<Sound>("ui_hover.mp3");
        uiButtonHover->volume = 0.5f;

        uiButtonClick = std::make_unique<Sound>("ui_click.mp3");
        uiButtonClick->volume = 0.5f;
    }
};
