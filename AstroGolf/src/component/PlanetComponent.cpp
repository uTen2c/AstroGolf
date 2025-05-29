#include "PlanetComponent.h"

#include <DxLib.h>
#include <spdlog/spdlog.h>

#include "../math/CircleCollider.h"
#include "../world/World.h"

PlanetComponent::PlanetComponent(const int id, const float radius): PhysicsComponent(id), planetGravity(0),
                                                                    radius(radius)
{
    collider = std::make_unique<CircleCollider>(radius);
    isStatic = true;
    planetGravity = 9.8f * radius;
}

void PlanetComponent::Update(const float deltaTime)
{
    PhysicsComponent::Update(deltaTime);

    if (planetGravity <= 0)
    {
        return;
    }

    const auto& player = world->GetPlayer();
    const auto distance = max(GetPlayerDistance() / 100, 1); // 1px = 1cm, convert cm to meter
    // const auto g = planetGravity * 0.2f / pow(distance, 1.2f);
    const auto g = planetGravity * 0.2f / distance;
    if (g < 20)
    {
        return;
    }
    const auto gravityDir = transform.translate.Copy().Sub(player->transform.translate).Normalize();
    player->gravitySources.emplace_back(gravityDir.Copy().Mul(g));
}

void PlanetComponent::Draw(DrawStack* stack)
{
    stack->Push();
    ApplyDrawStack(stack);

    const auto pos = stack->GetScreenPos();
    const auto scale = stack->GetScreenScale();
    const auto posnum = static_cast<int>(max(32 * scale.x, 32));
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
