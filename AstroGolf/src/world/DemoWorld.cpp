#include "DemoWorld.h"

#include <DxLib.h>

#include "../component/BoxComponent.h"
#include "../component/GoalHoleComponent.h"
#include "../component/RotatableBoxComponent.h"
#include "../component/object/SatelliteParentComponent.h"
#include "../component/planet/DemoPlanet2Component.h"
#include "../component/planet/DemoPlanetComponent.h"
#include "../game/StageManager.h"
#include "../math/Math.h"

DemoWorld::DemoWorld()
{
    background_graph_ = std::make_unique<Graph>("background.png", 2048, 2048);

    const auto box1 = std::make_shared<BoxComponent>(NextComponentId(), 500, 50);
    box1->transform.translate = {250, 500};

    const auto box2 = std::make_shared<BoxComponent>(NextComponentId(), 50, 500);
    box2->transform.translate = {550, 400};

    const auto box3 = std::make_shared<BoxComponent>(NextComponentId(), 50, 500);
    box3->transform.translate = {-100, 400};

    const auto box4 = std::make_shared<RotatableBoxComponent>(NextComponentId(), 50, 500);
    box4->transform.translate = {500, 400};
    box4->transform.rotation = 30 * Math::deg_to_rad;

    // const auto circle = std::make_shared<CircleComponent>(NextComponentId(), 64);
    // circle->transform.translate = {300, 0};

    const auto planet1 = std::make_shared<DemoPlanetComponent>(NextComponentId());
    planet1->transform.translate = {300, 0};

    const auto planet2 = std::make_shared<PlanetComponent>(NextComponentId(), 72);
    planet2->transform.translate = {600, -400};

    const auto planet3 = std::make_shared<DemoPlanet2Component>(NextComponentId());
    planet3->transform.translate = {-400, -100};
    planet3->transform.rotation = 65.0f * Math::deg_to_rad;

    AddComponent(box1);
    AddComponent(box2);
    AddComponent(box3);
    AddComponent(box4);
    AddComponent(planet1);
    AddComponent(planet2);
    AddComponent(planet3);

    const auto hole = std::make_shared<GoalHoleComponent>(NextComponentId());
    hole->parent = planet3;
    hole->transform.translate = {0, -175 + 40};
    AddComponent(hole);

    GetPlayer()->transform.translate = {250, 400};
}

DemoWorld::~DemoWorld() = default;

WorldType DemoWorld::GetType() const
{
    return WorldType::Demo;
}

void DemoWorld::Update(const float& deltaTime)
{
    World::Update(deltaTime);
    UpdateCamera(deltaTime);
}

std::string DemoWorld::GetStageId() const
{
    return StageManager::demoId;
}

void DemoWorld::DrawBackground(DrawStack& stack) const
{
    background_graph_->Draw(0, 0);
}

void DemoWorld::UpdateCamera(const float& deltaTime) const
{
    auto& camera = GetCamera();
    const auto player = GetPlayer();
    const auto focusPos = player->transform.translate;
    camera.transform.translate = Math::Lerp(camera.transform.translate, focusPos, deltaTime * 3.0f);
}
