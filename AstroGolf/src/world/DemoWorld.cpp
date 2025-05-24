#include "DemoWorld.h"

#include <DxLib.h>

#include "../component/BoxComponent.h"
#include "../component/RotatableBoxComponent.h"
#include "../component/planet/DemoPlanet2Component.h"
#include "../component/planet/DemoPlanetComponent.h"
#include "../graph/GraphUtils.h"
#include "../math/Math.h"

DemoWorld::DemoWorld()
{
    background_graph_handle_ = GraphUtils::Load("background.png");

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

    AddComponent(box1);
    AddComponent(box2);
    AddComponent(box3);
    AddComponent(box4);
    AddComponent(planet1);
    AddComponent(planet2);
    AddComponent(planet3);

    GetPlayer()->transform.translate = {250, 400};
}

DemoWorld::~DemoWorld()
{
    DeleteGraph(background_graph_handle_);
}

WorldType DemoWorld::GetType() const
{
    return WorldType::Demo;
}

void DemoWorld::DrawBackground(DrawStack& stack) const
{
    stack.Push();

    DrawGraphF(0, 0, background_graph_handle_, true);

    stack.Pop();
}
