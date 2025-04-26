#include "PlayerComponent.h"

#include <DxLib.h>
#include <imgui.h>

PlayerComponent::PlayerComponent(const int id): PhysicsComponent(id)
{
    SetCollider(circle_collider_);
}

void PlayerComponent::Update(const float deltaTime)
{
    PhysicsComponent::Update(deltaTime);

    auto moveVec = Vec2(0, 30);
    moveVec.Multiply(deltaTime);
    Move(moveVec);
}

void PlayerComponent::Draw(DrawStack* stack) const
{
    stack->Push();
    transform.ApplyDrawStack(stack);

    const auto& screenPos = stack->GetScreenPos();
    // 本体描画
    DrawCircleAA(screenPos.x, screenPos.y, circle_collider_.GetRadius() - 2, 16, GetColor(255, 255, 255), true);
    // コライダー描画
    DrawCircleAA(screenPos.x, screenPos.y, circle_collider_.GetRadius() + 1, 16, GetColor(0, 255, 0), false);

    stack->Pop();
}
