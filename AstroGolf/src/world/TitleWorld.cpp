#include "TitleWorld.h"

#include <algorithm>
#include <complex>
#include <DxLib.h>
#include <memory>
#include <spdlog/spdlog.h>

#include "../Game.h"
#include "../component/RotatableBoxComponent.h"
#include "../component/TestSquareComponent.h"
#include "../component/title/TitleComponent.h"
#include "../component/title/TitleWorldGroundComponent.h"
#include "../graph/GraphUtils.h"

TitleWorld::TitleWorld()
{
    background_graph_handle_ = GraphUtils::Load("background.png");
    title_graph_ = std::make_unique<Graph>("title.png", 1024, 1024);

    zoomEnabled = false;

    // const auto title = std::make_shared<TitleComponent>(NextComponentId());
    // title->transform.translate.y = 200;
    // title->transform.translate.x = static_cast<float>(WINDOW_WIDTH) * 0.5f;
    // AddComponent(title);

    const auto ground = std::make_shared<TitleWorldGroundComponent>(NextComponentId());
    ground->transform.translate = {0, 400};
    AddComponent(ground);

    // const auto testSquare = std::make_shared<TestSquareComponent>(NextComponentId());
    // testSquare->transform.translate = {0, 0};
    // AddComponent(testSquare);
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
}

void TitleWorld::OnCameraMove(CameraComponent* camera)
{
    constexpr auto min = (static_cast<float>(WINDOW_WIDTH) / 2.0f) - 1024;
    const auto max = std::abs(min);
    camera->transform.translate.x = std::clamp(camera->transform.translate.x, min, max);
}

void TitleWorld::DrawBackground(DrawStack& stack) const
{
    DrawGraphF(0, 0, background_graph_handle_, true);
    title_graph_->DrawCenter(WINDOW_WIDTH * 0.5f, 200);
}

WorldType TitleWorld::GetType() const
{
    return WorldType::Title;
}
