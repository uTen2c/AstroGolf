#include "StageSelectComponent.h"

#include <complex>
#include <DxLib.h>
#include <string>
#include <spdlog/spdlog.h>

#include "../../Game.h"
#include "../../game/SaveManager.h"
#include "../../game/StageManager.h"
#include "../../graph/Graphs.h"
#include "../../sound/Sounds.h"

namespace
{
    constexpr auto BUTTON_WIDTH = 500;
    constexpr auto BUTTON_HEIGHT = 80;
    constexpr auto LAYOUT_X_PADDING = 50;
    constexpr auto LAYOUT_Y_PADDING = 24;
    constexpr auto LAYOUT_DISPLAY_ITEM_COUNT = 7;
    constexpr auto LAYOUT_Y_GAP = (WINDOW_HEIGHT - LAYOUT_Y_PADDING * 2 - (BUTTON_HEIGHT * LAYOUT_DISPLAY_ITEM_COUNT)) /
        (LAYOUT_DISPLAY_ITEM_COUNT - 1);
    constexpr float BUTTON_LABEL_PADDING = 16;
    constexpr auto FONT_SIZE = 24;

    bool graph_initialized = false;
    std::unique_ptr<Graph> buttons_graph;
}

StageSelectComponent::StageSelectComponent(const int id)
    : Component(id)
{
    font_handle_ = CreateFontToHandle("M PLUS 1p Medium", FONT_SIZE, 5, DX_FONTTYPE_ANTIALIASING_8X8);

    if (!graph_initialized)
    {
        buttons_graph = std::make_unique<Graph>("stage_select/buttons.png", BUTTON_WIDTH, BUTTON_HEIGHT);
    }
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
            Sounds::uiButtonClick->Play();
            OnSelect(clicked_index_);
        }
        clicked_index_ = -1;
    }
}

void StageSelectComponent::Draw(DrawStack* stack)
{
    int hoveringIndex = -1;
    const auto& mousePos = Game::Device().MousePos();
    const auto& stages = StageManager::GetStages();
    for (int i = 0; i < stages.size(); ++i)
    {
        if (i < current_center_index_)
        {
            continue;
        }

        const auto startY = i - current_center_index_;
        const auto y = 280 + startY * (BUTTON_HEIGHT + LAYOUT_Y_GAP);
        const auto selected = focused_index_ == -1
                                  ? i == current_center_index_
                                  : i == focused_index_;
        const auto& stage = stages[i];
        const auto& progress = SaveManager::GetProgresses()[stage.id];

        DrawButton(0, static_cast<float>(y), stage.name, selected, progress.clearedChallenges.size());

        if (!Game::HasFocus())
        {
            continue;
        }
        if (mousePos.x > BUTTON_WIDTH)
        {
            continue;
        }
        if (mousePos.y < y || mousePos.y > y + BUTTON_HEIGHT)
        {
            continue;
        }

        hoveringIndex = i;
    }

    mouse_hovering_index_ = hoveringIndex;
    if (hoveringIndex != -1)
    {
        focused_index_ = hoveringIndex;

        if (focused_index_ != last_focused_index_)
        {
            Sounds::uiButtonHover->Play();
        }
        last_focused_index_ = focused_index_;
    }
}

int StageSelectComponent::GetFocusedIndex() const
{
    return focused_index_;
}

void StageSelectComponent::DrawButton(const float& x, const float& y, const std::string& label, const bool& selected,
                                      const int& stars) const
{
    static constexpr float bottom_border_width = 10;
    const float offsetX = selected ? 0 : -16;
    buttons_graph->Draw(x + offsetX, y, 0, selected ? 1 : 0);
    DrawStringFToHandle(
        x + BUTTON_LABEL_PADDING,
        y + (BUTTON_HEIGHT - bottom_border_width - static_cast<float>(FONT_SIZE)) * 0.5f,
        label.c_str(),
        GetColor(255, 255, 255),
        font_handle_
    );

    for (int i = 0; i < 3; ++i)
    {
        constexpr auto starGap = 8;
        const auto starX = x + BUTTON_WIDTH - i * (Graphs::starsGraph->width + starGap) - 64 + offsetX;
        const auto starY = y + (BUTTON_HEIGHT - bottom_border_width) * 0.5f;
        Graphs::starsGraph->DrawCenter(starX, starY, 2 - i < stars ? 1 : 0);
    }
}

int StageSelectComponent::CheckMoveKey(float deltaTime)
{
    const auto& mousePos = Game::Device().MousePos();

    if (mousePos.x > BUTTON_WIDTH + LAYOUT_X_PADDING * 2)
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
