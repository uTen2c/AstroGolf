#include "TitleWorld.h"

#include <algorithm>
#include <complex>
#include <DxLib.h>
#include <memory>
#include <spdlog/spdlog.h>

#include "StageSelectWorld.h"
#include "../Game.h"
#include "../component/GoalHoleComponent.h"
#include "../component/RotatableBoxComponent.h"
#include "../component/SimpleSquareComponent.h"
#include "../component/title/StartTextComponent.h"
#include "../component/title/TitleWorldGroundComponent.h"
#include "../game/StageManager.h"
#include "../graph/Graphs.h"
#include "../math/Math.h"

TitleWorld::TitleWorld()
{
    zoomEnabled = false;

    cursor_hint_font_handle_ = CreateFontToHandle("M PLUS 1p Medium", 24, 5, DX_FONTTYPE_ANTIALIASING_8X8);
}

TitleWorld::~TitleWorld()
{
    DeleteFontToHandle(cursor_hint_font_handle_);
}

void TitleWorld::Init()
{
    StageWorld::Init();

    const auto ground = std::make_shared<TitleWorldGroundComponent>(NextComponentId());
    ground->transform.translate = {0, 600};
    AddComponent(ground);

    const auto rightSlope = std::make_shared<RotatableBoxComponent>(NextComponentId(), 200, 800);
    rightSlope->zIndex = 0;
    rightSlope->transform.rotation = 74.0f * Math::deg_to_rad;
    rightSlope->transform.translate = {1400, 300};
    AddComponent(rightSlope);

    const auto rightWall = std::make_shared<RotatableBoxComponent>(NextComponentId(), 200, 1000);
    rightWall->transform.translate = {1300, -200};
    AddComponent(rightWall);

    const auto leftSlope = std::make_shared<RotatableBoxComponent>(NextComponentId(), 200, 800);
    leftSlope->zIndex = 0;
    leftSlope->transform.rotation = -74.0f * Math::deg_to_rad;
    leftSlope->transform.translate = {-1400, 300};
    AddComponent(leftSlope);

    const auto leftWall = std::make_shared<RotatableBoxComponent>(NextComponentId(), 200, 1000);
    leftWall->transform.translate = {-1300, -200};
    AddComponent(leftWall);

    const auto hole = std::make_shared<GoalHoleComponent>(NextComponentId());
    hole->transform.translate = {0, 340};
    AddComponent(hole);

    const auto startText = std::make_shared<StartTextComponent>(NextComponentId());
    startText->transform.translate = {0, 128};
    AddComponent(startText);

    GetPlayer()->transform.translate = {500, 0};
    // ResetCamera();
}

void TitleWorld::Update(const float& deltaTime)
{
    World::Update(deltaTime);

    UpdateCamera(deltaTime);

    opening_seconds_ += deltaTime;

    // 表示されてから5秒間はカーソルのヒントを表示しない
    should_show_cursor_hint_ = opening_seconds_ > 5;

    if (should_show_cursor_hint_)
    {
        static constexpr auto max_cursor_seconds = 2.0f;
        if (cursor_animation_seconds_ >= max_cursor_seconds)
        {
            cursor_animation_seconds_ = 0;
        }
        else
        {
            cursor_animation_seconds_ = min(cursor_animation_seconds_ + deltaTime, max_cursor_seconds);
        }
        cursor_animation_delta_ = cursor_animation_seconds_ / max_cursor_seconds;
        cursor_fade_in_delta_ = max(min(cursor_fade_in_delta_ + cursor_animation_delta_, 1.0f), cursor_fade_in_delta_);
    }
}

void TitleWorld::OnCameraMoveWithMouse(CameraComponent* camera)
{
    camera->transform.translate = ClampCameraPos(camera->transform.translate);
}

void TitleWorld::OnGoal()
{
    Game::instance->ChangeWorldWithTransition<StageSelectWorld>(TransitionMode::Circle);
}

std::string TitleWorld::GetStageId() const
{
    return StageManager::titleId;
}

void TitleWorld::DrawBackground(DrawStack& stack) const
{
    Graphs::stageBackground->Draw(0, 0);
    // title_graph_->DrawCenter(WINDOW_WIDTH * 0.5f, 200.0f);
    const auto titleY = max(-GetCamera().transform.translate.y + 200.0f, 200);
    Graphs::titleGraph->DrawCenter(WINDOW_WIDTH * 0.5f, titleY);
}

void TitleWorld::PostDraw(DrawStack& stack) const
{
    if (Game::instance->isPaused || !should_show_cursor_hint_)
    {
        return;
    }

    const auto& player = GetPlayer();
    if (player->GetShotCount() > 0 || player->isDragging)
    {
        return;
    }

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * cursor_fade_in_delta_));


    stack.Push();

    stack.Translate({player->transform.translate.x + 50, 308});

    stack.Push();

    const auto offsetX = Math::Lerp(0, 64, Math::EaseOutQuad(cursor_animation_delta_));
    stack.Translate({offsetX, offsetX / 2});
    Graphs::titleCursorGraph->Draw(stack);

    stack.Pop();

    stack.Push();

    stack.Translate({Graphs::titleCursorGraph->width * 1.5f, -8});
    const auto& screenPos = stack.GetScreenPos();
    static constexpr auto hint = "ボールをドラッグしてショット";
    // DrawStringFToHandle(screenPos.x, screenPos.y, hint, GetColor(255, 255, 255), cursor_hint_font_handle_);


    stack.Pop();

    stack.Pop();

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleWorld::UpdateCamera(const float& deltaTime) const
{
    auto& camera = GetCamera();
    const auto player = GetPlayer();
    const auto focusPos = player->transform.translate.Copy().Add({0, -300});

    if (GetOpeningDelta() < 0.5f)
    {
        camera.transform.translate = focusPos + Vec2(0, -720);
        return;
    }

    camera.transform.translate = Math::Lerp(camera.transform.translate, ClampCameraPos(focusPos), deltaTime * 3.0f);
}

void TitleWorld::ResetCamera() const
{
    const auto focusPos = GetPlayer()->transform.translate.Copy().Add({0, -300});
    GetCamera().transform.translate = ClampCameraPos(focusPos);
}

Vec2 TitleWorld::ClampCameraPos(const Vec2& pos)
{
    constexpr auto min = (static_cast<float>(WINDOW_WIDTH) / 2.0f) - 1024;
    const auto max = std::abs(min);
    return {
        std::clamp(pos.x, min, max),
        std::clamp(pos.y, 0.0f, 32.0f),
    };
}

float TitleWorld::GetOpeningDelta() const
{
    static constexpr float max_duration = 0.25f;
    return std::clamp(opening_seconds_, 0.0f, max_duration) / max_duration;
}

WorldType TitleWorld::GetType() const
{
    return WorldType::Title;
}
