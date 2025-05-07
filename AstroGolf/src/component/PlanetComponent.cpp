#include "PlanetComponent.h"

#include <DxLib.h>

#include "../math/CircleCollider.h"
#include "../world/World.h"

PlanetComponent::PlanetComponent(const int id, const float radius): PhysicsComponent(id), platnetGravity(0),
                                                                    radius(radius)
{
    collider = std::make_unique<CircleCollider>(radius);
    isStatic = true;
}

void PlanetComponent::Update(const float deltaTime)
{
    PhysicsComponent::Update(deltaTime);

    if (platnetGravity <= 0)
    {
        return;
    }

    const auto distance = GetPlayerDistance();
    const auto& player = world->GetPlayer();
    player->gravityVelocity
}

void PlanetComponent::Draw(DrawStack* stack)
{
    stack->Push();
    transform.ApplyDrawStack(stack);

    const auto pos = stack->GetScreenPos();
    const auto scale = stack->GetScreenScale();
    const auto posnum = max(32 * scale.x, 32);
    DrawCircleAA(pos.x, pos.y, radius * scale.x, posnum, 0xFFFFFFFF, true);

    const auto distance = GetPlayerDistance();
    const auto textWidth = GetDrawFormatStringWidth("%0.3f", distance);
    DrawFormatString(static_cast<int>(pos.x),
                     static_cast<int>(pos.y - static_cast<float>(textWidth) * 0.5f), GetColor(0, 0, 0),
                     "%0.3f", distance);
    stack->Pop();
}

float PlanetComponent::GetPlayerDistance() const
{
    return transform.translate.Distance(world->GetPlayer()->transform.translate) - radius - world->GetPlayer()->radius;
}
