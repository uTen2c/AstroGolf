#include "StageSelectComponent.h"

#include <complex>
#include <DxLib.h>
#include <string>
#include <spdlog/spdlog.h>

#include "../../Game.h"

namespace
{
    constexpr auto BUTTON_WIDTH = 500;
    constexpr auto BUTTON_HEIGHT = 72;
    constexpr auto BUTTON_BORDER_WIDTH = 3;
    constexpr auto LAYOUT_X_PADDING = 50;
    constexpr auto LAYOUT_Y_PADDING = 24;
    constexpr auto LAYOUT_DISPLAY_ITEM_COUNT = 7;
    constexpr auto LAYOUT_Y_GAP = (WINDOW_HEIGHT - LAYOUT_Y_PADDING * 2 - (BUTTON_HEIGHT * LAYOUT_DISPLAY_ITEM_COUNT)) /
        (LAYOUT_DISPLAY_ITEM_COUNT - 1);
}

StageSelectComponent::StageSelectComponent(const int id): Component(id)
{
}

void StageSelectComponent::Update(float delta)
{
    constexpr auto MAX = 9; // FIXME
    const auto moveDir = CheckMoveKey(delta);
    current_index_ = std::clamp(current_index_ + moveDir, 0, MAX);
}

void StageSelectComponent::Draw(DrawStack* stack)
{
    for (int i = 0; i < 10; ++i)
    {
        const auto scale = i == current_index_ ? 1.0f : 0.8f;
        const auto startY = i - current_index_ + 3;
        DrawButton(LAYOUT_X_PADDING, LAYOUT_Y_PADDING + startY * (BUTTON_HEIGHT + LAYOUT_Y_GAP), scale,
                   std::format("Test Item {}", i), i == current_index_);
    }
}

// TODO SetFontSizeが重すぎる
void StageSelectComponent::DrawButton(const float& x, const float& y, const float& scale, const std::string& label,
                                      const bool& selected)
{
    const auto scaledBorderWidth = BUTTON_BORDER_WIDTH * scale;
    const auto scaledWidth = BUTTON_WIDTH * scale;
    const auto scaledHeight = BUTTON_HEIGHT * scale;
    const auto scaledX = x + (BUTTON_WIDTH - scaledWidth) * 0.5f;
    const auto scaledY = y + (BUTTON_HEIGHT - scaledHeight) * 0.5f;


    const auto borderColor = selected ? GetColor(66, 86, 213) : GetColor(22, 33, 104);
    DrawBoxAA(scaledX, scaledY, scaledX + scaledWidth, scaledY + scaledHeight, borderColor, false, scaledBorderWidth);

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * 0.2f));
    DrawBoxAA(scaledX + scaledBorderWidth, scaledY + scaledBorderWidth,
              scaledX + scaledWidth - scaledBorderWidth, scaledY + scaledHeight - scaledBorderWidth,
              GetColor(0, 17, 206), true);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    const auto fontSize = static_cast<int>(24 * scale);
    SetFontSize(fontSize);
    const auto labelWidth = GetDrawStringWidth(label.c_str(), static_cast<int>(label.length()));
    DrawStringF(
        scaledX + (scaledWidth - static_cast<float>(labelWidth)) * 0.5f,
        scaledY + (scaledHeight - static_cast<float>(fontSize)) * 0.5f,
        label.c_str(),
        GetColor(255, 255, 255)
    );
}

int StageSelectComponent::CheckMoveKey(float deltaTime)
{
    auto pressedAny = false;
    auto moveDir = 0;
    const auto shouldRepeatInput = key_check_repeat_ ? key_check_duration_ > 0.1f : key_check_duration_ > 0.5f;
    spdlog::info("shouldRepeatInput: {}", shouldRepeatInput);

    if (CheckHitKey(KEY_INPUT_DOWN) != 0)
    {
        pressedAny = true;
        if (!last_down_pressed_ || shouldRepeatInput)
        {
            moveDir += 1;

            if (shouldRepeatInput)
            {
                key_check_repeat_ = true;
                key_check_duration_ = 0;
            }
        }
        last_down_pressed_ = true;
    }
    else
    {
        last_down_pressed_ = false;
    }

    if (CheckHitKey(KEY_INPUT_UP) != 0)
    {
        pressedAny = true;
        if (!last_up_pressed_ || shouldRepeatInput)
        {
            moveDir += -1;

            if (shouldRepeatInput)
            {
                key_check_repeat_ = true;
                key_check_duration_ = 0;
            }
        }
        last_up_pressed_ = true;
    }
    else
    {
        last_up_pressed_ = false;
    }

    if (pressedAny)
    {
        key_check_duration_ += deltaTime;
    }
    else
    {
        key_check_repeat_ = false;
        key_check_duration_ = 0;
    }

    spdlog::info("key_check_duration_: {}", key_check_duration_);

    return moveDir;
}
