#include "DemoPlanet2Component.h"

#include <DxLib.h>

namespace
{
    constexpr auto SPRITE_SIZE = 512.0f;
    constexpr auto PLANET_RADIUS = 350 / 2.0f;
}

DemoPlanet2Component::DemoPlanet2Component(const int id): PlanetComponent(id, PLANET_RADIUS)
{
    graph_ = std::make_unique<Graph>("planet_2.png", 512, 512);
}

DemoPlanet2Component::~DemoPlanet2Component() = default;

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
    DrawExtendGraphF(pos.x - offset, pos.y - offset, pos.x + offset, pos.y + offset, graph_->handle, true);
    stack->Pop();
}
