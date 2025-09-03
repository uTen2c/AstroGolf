#include "PlanetComponent.h"

#include <DxLib.h>
#include <spdlog/spdlog.h>

#include "../Game.h"
#include "../math/collider/CircleCollider.h"
#include "../world/World.h"

PlanetComponent::PlanetComponent(const int id, const float radius): PhysicsComponent(id), planetGravity(0),
                                                                    radius(radius)
{
    collider = std::make_unique<CircleCollider>(radius);
    is_static = true;
    planetGravity = 9.8f * radius;
}

void PlanetComponent::Update(const float deltaTime)
{
    PhysicsComponent::Update(deltaTime);
}

void PlanetComponent::Draw(DrawStack* stack)
{
    stack->Push();
    ApplyDrawStack(stack);

    const auto pos = stack->GetScreenPos();
    const auto scale = stack->GetScreenScale();
    const auto posnum = static_cast<int>(max(32 * scale.x, 32));
    DrawCircleAA(pos.x, pos.y, radius * scale.x, posnum, 0xFFFFFFFF, true);

    // デバッグ表示
    if (Game::debugEnabled || world->GetType() == WorldType::Editor)
    {
        const auto distance = GetPlayerDistance(world->GetPlayer());
        const auto textWidth = GetDrawFormatStringWidth("%0.3f", distance);
        DrawFormatString(static_cast<int>(pos.x),
                         static_cast<int>(pos.y - static_cast<float>(textWidth) * 0.5f), GetColor(0, 0, 0),
                         "%0.3f", distance);
    }
    stack->Pop();
}

void PlanetComponent::ApplyGravity(PhysicsComponent* component) const
{
    if (planetGravity <= 0)
    {
        return;
    }

    const auto distance = max(GetPlayerDistance(component) / 100, 1); // 1px = 1cm, convert cm to meter
    const auto g = planetGravity * 0.2f / distance;
    if (g < 20)
    {
        return;
    }
    const auto gravityDir = transform.translate.Copy().Sub(component->transform.translate).Normalize();
    component->gravity_sources.emplace_back(gravityDir.Copy().Mul(g));
}

float PlanetComponent::GetPlayerDistance(const PhysicsComponent* component) const
{
    // GetPlayerでプレイヤーの半径を取得しているが、弾道も同じ処理をしている
    return transform.translate.Distance(component->transform.translate) - radius - world->GetPlayer()->radius;
}
