#include "DemoPlanet2Component.h"

#include <DxLib.h>

#include "../../graph/GraphUtils.h"

namespace
{
    constexpr auto SPRITE_SIZE = 512.0f;
    constexpr auto PLANET_RADIUS = 350 / 2.0f;
}

DemoPlanet2Component::DemoPlanet2Component(const int id): PlanetComponent(id, PLANET_RADIUS)
{
    graph_handle_ = GraphUtils::Load("planet_2.png");
}

DemoPlanet2Component::~DemoPlanet2Component()
{
    DeleteGraph(graph_handle_);
}

void DemoPlanet2Component::Update(const float deltaTime)
{
    PlanetComponent::Update(deltaTime);
}

void DemoPlanet2Component::Draw(DrawStack* stack)
{
    stack->Push();
    transform.ApplyDrawStack(stack);

    const auto pos = stack->GetScreenPos();
    const auto scale = stack->GetScreenScale().x;
    const auto scaledSpriteSize = SPRITE_SIZE * scale;
    const auto offset = scaledSpriteSize / 2.0f;
    DrawExtendGraphF(pos.x- offset, pos.y - offset, pos.x + offset, pos.y + offset, graph_handle_, true);
    stack->Pop();
}
