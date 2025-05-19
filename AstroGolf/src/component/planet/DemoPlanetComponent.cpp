#include "DemoPlanetComponent.h"

#include <DxLib.h>

#include "../../GraphUtils.h"

namespace
{
    constexpr auto sprite_size = 512.0f;
    constexpr auto planet_radius = 500 / 2.0f;
}

DemoPlanetComponent::DemoPlanetComponent(const int id): PlanetComponent(id, planet_radius)
{
    graph_handle_ = GraphUtils::Load("planet_1.png");
}

DemoPlanetComponent::~DemoPlanetComponent()
{
    DeleteGraph(graph_handle_);
}

void DemoPlanetComponent::Update(const float deltaTime)
{
    PlanetComponent::Update(deltaTime);
}

void DemoPlanetComponent::Draw(DrawStack* stack)
{
    stack->Push();
    transform.ApplyDrawStack(stack);

    const auto pos = stack->GetScreenPos();
    const auto scale = stack->GetScreenScale().x;
    const auto scaledSpriteSize = sprite_size * scale;
    const auto offset = scaledSpriteSize / 2.0f;
    DrawExtendGraphF(pos.x- offset, pos.y - offset, pos.x + offset, pos.y + offset, graph_handle_, true);
    stack->Pop();
}
