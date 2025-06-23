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
    spdlog::info("constructor");
    title_graph_ = std::make_unique<Graph>("title.png", 1024, 1024);

    zoomEnabled = false;
}

TitleWorld::~TitleWorld() = default;

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

void TitleWorld::LoadResources()
{
}

void TitleWorld::DrawBackground(DrawStack& stack) const
{
    Graphs::stageBackground->Draw(0, 0);
    // title_graph_->DrawCenter(WINDOW_WIDTH * 0.5f, 200.0f);
    const auto titleY = max(-GetCamera().transform.translate.y + 200.0f, 200);
    title_graph_->DrawCenter(WINDOW_WIDTH * 0.5f, titleY);
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
