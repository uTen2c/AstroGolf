#include "TitleWorld.h"

#include <algorithm>
#include <complex>
#include <DxLib.h>
#include <memory>
#include <spdlog/spdlog.h>

#include "../Game.h"
#include "../component/GoalHoleComponent.h"
#include "../component/RotatableBoxComponent.h"
#include "../component/SimpleSquareComponent.h"
#include "../component/TestSquareComponent.h"
#include "../component/title/TitleWorldGroundComponent.h"
#include "../graph/GraphUtils.h"
#include "../math/Math.h"

TitleWorld::TitleWorld()
{
    background_graph_handle_ = GraphUtils::Load("background.png");
    title_graph_ = std::make_unique<Graph>("title.png", 1024, 1024);

    zoomEnabled = false;

    const auto ground = std::make_shared<TitleWorldGroundComponent>(NextComponentId());
    ground->transform.translate = {0, 400};
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
    hole->transform.translate = {200, 340};
    // hole->transform.rotation = 30.0f * Math::deg_to_rad;
    AddComponent(hole);

    const auto parent = std::make_shared<SimpleSquareComponent>(NextComponentId());
    parent->transform.translate = {0, 100};
    parent->transform.rotation = 45.0f * Math::deg_to_rad;

    const auto child = std::make_shared<SimpleSquareComponent>(NextComponentId());
    child->parent = parent;
    child->transform.translate = {0, 100};

    const auto hole2 = std::make_shared<GoalHoleComponent>(NextComponentId());
    hole2->parent = parent;
    hole2->transform.translate = {0, 100};
    // hole->transform.rotation = 30.0f * Math::deg_to_rad;
    AddComponent(hole2);

    AddComponent(parent);
    AddComponent(child);
    AddComponent(hole2);
}

TitleWorld::~TitleWorld()
{
    DeleteGraph(background_graph_handle_);
}

void TitleWorld::Update(const float& deltaTime)
{
    constexpr auto base = 9.8f * 300;
    const auto playerDistance = std::clamp(200.0f - GetPlayer()->transform.translate.y, 0.0f, 50.0f);
    const auto distance = max(playerDistance / 100.0f, 1.0f); // 1px = 1cm, convert cm to meter
    const auto g = base * 0.2f / distance;
    GetPlayer()->gravitySources.emplace_back(0.0f, g);

    World::Update(deltaTime);

    UpdateCamera(deltaTime);
}

void TitleWorld::OnCameraMoveWithMouse(CameraComponent* camera)
{
    camera->transform.translate = ClampCameraPos(camera->transform.translate);
}

void TitleWorld::DrawBackground(DrawStack& stack) const
{
    DrawGraphF(0, 0, background_graph_handle_, true);
    title_graph_->DrawCenter(WINDOW_WIDTH * 0.5f, 200);
}

void TitleWorld::UpdateCamera(const float& deltaTime)
{
    auto& camera = GetCamera();
    const auto player = GetPlayer();
    const auto focusPos = player->transform.translate.Copy().Add({0, -300});
    camera.transform.translate = Math::Lerp(camera.transform.translate, ClampCameraPos(focusPos), deltaTime * 3.0f);
}

Vec2 TitleWorld::ClampCameraPos(const Vec2& pos)
{
    // return pos;
    constexpr auto min = (static_cast<float>(WINDOW_WIDTH) / 2.0f) - 1024;
    const auto max = std::abs(min);
    return {
        std::clamp(pos.x, min, max),
        std::clamp(pos.y, 0.0f, 32.0f),
    };
}

WorldType TitleWorld::GetType() const
{
    return WorldType::Title;
}
