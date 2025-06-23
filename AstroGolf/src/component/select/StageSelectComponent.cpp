#include "StageSelectComponent.h"

#include <complex>
#include <DxLib.h>
#include <string>
#include <spdlog/spdlog.h>

#include "../../Game.h"
#include "../../game/SaveManager.h"
#include "../../game/StageManager.h"
#include "../../graph/Graphs.h"

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
    constexpr auto FONT_SIZE = 24;
}

StageSelectComponent::StageSelectComponent(const int id): Component(id)
{
    font_handle_ = CreateFontToHandle("M PLUS 1p Medium", FONT_SIZE, 5, DX_FONTTYPE_ANTIALIASING_8X8);
}

StageSelectComponent::~StageSelectComponent()
{
    DeleteFontToHandle(font_handle_);
}

void StageSelectComponent::Update(float delta)
{
    const auto dir = CheckMoveKey(delta);
    const int max = static_cast<int>(StageManager::GetStages().size()) - 1;
    current_center_index_ = std::clamp(current_center_index_ + dir, 0, max);

    if (Game::Device().LeftClicked() && mouse_hovering_index_ != -1 && clicked_index_ == -1)
    {
        clicked_index_ = mouse_hovering_index_;
        mouse_clicked_ = true;
    }
    else
    {
        mouse_clicked_ = false;
    }

    if (Game::Device().LeftReleased())
    {
        if (mouse_hovering_index_ != -1 && mouse_hovering_index_ == clicked_index_)
        {
            OnSelect(clicked_index_);
        }
        clicked_index_ = -1;
    }
}

void StageSelectComponent::Draw(DrawStack* stack)
{
    int mouseX;
    int mouseY;
    int hoveringIndex = -1;
    GetMousePoint(&mouseX, &mouseY);
    const auto& stages = StageManager::GetStages();
    for (int i = 0; i < stages.size(); ++i)
    {
        const auto startY = i - current_center_index_ + 3;
        const auto y = LAYOUT_Y_PADDING + startY * (BUTTON_HEIGHT + LAYOUT_Y_GAP);
        const auto selected = focused_index_ == -1
                                  ? i == current_center_index_
                                  : i == focused_index_;
        const auto scale = selected ? 1.0f : 0.8f;

        const auto& stage = stages[i];
        const auto& progress = SaveManager::GetProgresses()[stage.id];
        DrawButton(LAYOUT_X_PADDING, static_cast<float>(y), scale, stage.name, selected,
                   progress.clearedChallenges.size());

        if (mouseX < LAYOUT_X_PADDING || mouseX > LAYOUT_X_PADDING + BUTTON_WIDTH)
        {
            continue;
        }
        if (mouseY < y || mouseY > y + BUTTON_HEIGHT)
        {
            continue;
        }

        hoveringIndex = i;
    }

    mouse_hovering_index_ = hoveringIndex;
    if (hoveringIndex != -1)
    {
        focused_index_ = hoveringIndex;
    }
}

int StageSelectComponent::GetFocusedIndex() const
{
    return focused_index_;
}

void StageSelectComponent::DrawButton(
    const float& x, const float& y,
    const float& scale, const std::string& label, const bool& selected, const int& stars
) const
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

    const auto labelWidth = GetDrawStringWidthToHandle(label.c_str(), static_cast<int>(label.length()), font_handle_);
    DrawStringFToHandle(
        scaledX + (scaledWidth - static_cast<float>(labelWidth)) * 0.5f,
        scaledY + (scaledHeight - static_cast<float>(FONT_SIZE)) * 0.5f + (selected ? -2.f : 1.f),
        label.c_str(),
        GetColor(255, 255, 255),
        font_handle_
    );

    for (int i = 0; i < 3; ++i)
    {
        constexpr auto starGap = 8;
        const auto starX = scaledX + scaledWidth - i * (Graphs::starsGraph->width + starGap);
        Graphs::starsGraph->DrawCenter(starX, scaledY, 2 - i < stars ? 1 : 0);
    }
}

int StageSelectComponent::CheckMoveKey(float deltaTime)
{
    int mouseX;
    int mouseY;
    GetMousePoint(&mouseX, &mouseY);

    if (mouseX > BUTTON_WIDTH + LAYOUT_X_PADDING * 2)
    {
        return 0;
    }

    auto wheelRotVol = GetMouseWheelRotVol();
    wheelRotVol *= -1;

    if (wheelRotVol > 0)
    {
        return 1;
    }

    if (wheelRotVol < 0)
    {
        return -1;
    }
    return 0;
}

void StageSelectComponent::OnSelect(int index)
{
    const auto& stages = StageManager::GetStages();
    const auto& stage = stages[index];
    spdlog::info("Select {}, {}, {}", index, stage.id, stage.name);
    if (auto world = StageManager::CreateWorld(stage.id))
    {
        Game::instance->ChangeWorldWithTransition(TransitionMode::Slide, std::move(world));
    }
}
