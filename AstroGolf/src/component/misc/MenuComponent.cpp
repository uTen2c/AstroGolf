#include "MenuComponent.h"

#include <DxLib.h>
#include <spdlog/spdlog.h>

#include "../../Game.h"
#include "../../math/BoundingBox.h"
#include "../../math/Math.h"
#include "../../world/StageSelectWorld.h"
#include "../../world/StageWorld.h"

namespace
{
    bool graph_initialized = false;
    std::unique_ptr<Graph> menu_button_graph;
    std::unique_ptr<Graph> menu_background_graph;
    std::unique_ptr<Graph> menu_buttons_graph;
}

MenuComponent::MenuComponent(const int id): Component(id)
{
    zIndex = 3000;
    updateWhenPaused = true;

    if (!graph_initialized)
    {
        menu_button_graph = std::make_unique<Graph>("menu_button.png", 72, 72);
        menu_background_graph = std::make_unique<Graph>("menu/menu_background.png", 1024, 720);
        menu_buttons_graph = std::make_unique<Graph>("menu/buttons.png", 512, 70);
        graph_initialized = true;
    }
}

void MenuComponent::SetMenuOpen(const bool open)
{
    menu_opened_ = open;
    Game::instance->isPaused = open;
    menu_transition_delta_ = 0;
}

void MenuComponent::Update(float delta)
{
    // Esc押したら開閉する
    if (Game::Device().menuKey.Pressed())
    {
        SetMenuOpen(!menu_opened_);
    }
}

void MenuComponent::Draw(DrawStack* stack)
{
    DrawMenuButton();
    DrawMenu();
}

void MenuComponent::DrawMenuButton()
{
    if (menu_opened_)
    {
        menu_button_clicked_ = false;
        return;
    }

    int mouseX;
    int mouseY;
    GetMousePoint(&mouseX, &mouseY);

    constexpr auto menuButtonPadding = 24;
    const auto halfW = static_cast<float>(menu_button_graph->width) * 0.5f;
    const auto halfH = static_cast<float>(menu_button_graph->height) * 0.5f;
    const auto centerX = WINDOW_WIDTH - halfW - menuButtonPadding;
    const auto centerY = halfH + menuButtonPadding;
    const auto hovering = Math::InRange(static_cast<float>(mouseX), centerX - halfW, centerX + halfW) &&
        Math::InRange(static_cast<float>(mouseY), centerY - halfH, centerY + halfH);
    menu_button_graph->DrawCenter(centerX, centerY, hovering ? 1 : 0, 0);

    if (Game::Device().LeftClicked() && hovering && !menu_button_clicked_)
    {
        menu_button_clicked_ = true;
    }

    if (Game::Device().LeftReleased() && hovering && menu_button_clicked_)
    {
        menu_button_clicked_ = false;
        SetMenuOpen(true);
    }
}

void MenuComponent::DrawMenu()
{
    if (!menu_opened_)
    {
        return;
    }

    int mouseX;
    int mouseY;
    GetMousePoint(&mouseX, &mouseY);

    menu_background_graph->Draw(WINDOW_WIDTH - menu_background_graph->width, 0);

    constexpr float padding = 24;
    const float buttonX = WINDOW_WIDTH - menu_buttons_graph->width - padding;
    const float baseY = WINDOW_HEIGHT - menu_buttons_graph->height - padding;
    const float gap = menu_buttons_graph->height + 24;


    float offset = gap * -3;

    // ゲームに戻る
    DrawMenuItem(buttonX, baseY + offset, 0, [this]
    {
        SetMenuOpen(false);
    });

    offset += gap;

    // はじめから
    DrawMenuItem(buttonX, baseY + offset, 1, [this]
    {
        if (const auto stageWorld = dynamic_cast<StageWorld*>(world))
        {
            spdlog::info("stageWorld");
            stageWorld->Reload();
        }
    });

    offset += gap;

    // ステージセレクト
    DrawMenuItem(buttonX, baseY + offset, 2, []
    {
        Game::instance->ChangeWorldWithTransition<StageSelectWorld>(TransitionMode::Circle);
    });

    offset += gap;

    // ゲームを閉じる
    DrawMenuItem(buttonX, baseY + offset, 3, []
    {
        SetWindowVisibleFlag(false);
        Game::shouldShutdown = true;
    });

    menu_transition_delta_ += Game::deltaTime;

    if (Game::Device().LeftReleased())
    {
        clicked_index_ = -1;
    }
}

void MenuComponent::DrawMenuItem(const float x, const float y, const int buttonIndex,
                                 const std::function<void()>& onClick)
{
    int mouseX;
    int mouseY;
    GetMousePoint(&mouseX, &mouseY);

    const auto tileY = buttonIndex * 2;
    const auto& bb = BoundingBox(menu_buttons_graph->width, menu_buttons_graph->height);
    const bool& hovering = bb.Contains(
        Vec2(x, y) + Vec2(menu_buttons_graph->width / 2.0f, menu_buttons_graph->height / 2.0f),
        Vec2(mouseX, mouseY)
    );

    menu_buttons_graph->Draw(
        hovering ? x - 4 : x,
        y,
        0,
        hovering ? tileY + 1 : tileY
    );

    if (Game::Device().LeftClicked() && hovering && clicked_index_ == -1)
    {
        clicked_index_ = buttonIndex;
    }

    if (Game::Device().LeftReleased() && hovering && clicked_index_ == buttonIndex)
    {
        clicked_index_ = -1;
        if (onClick)
        {
            onClick();
        }
    }
}
