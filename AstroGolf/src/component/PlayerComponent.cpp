#include "PlayerComponent.h"

#include <DxLib.h>
#include <imgui.h>

#include "../Game.h"
#include "../math/CircleCollider.h"

PlayerComponent::PlayerComponent(const int id): PhysicsComponent(id)
{
    collider = std::make_unique<CircleCollider>(32);
    gravity = 9.8f * 100;
    gravityVelocity = {0, gravity};
}

void PlayerComponent::Update(const float deltaTime)
{
    PhysicsComponent::Update(deltaTime);

    auto move = Vec2(0, 0);

    if (CheckHitKey(KEY_INPUT_A))
    {
        move.x -= 1;
    }
    if (CheckHitKey(KEY_INPUT_D))
    {
        move.x += 1;
    }
    if (CheckHitKey(KEY_INPUT_W))
    {
        move.y -= 1;
    }
    if (CheckHitKey(KEY_INPUT_S))
    {
        move.y += 1;
    }

    if (CheckHitKey(KEY_INPUT_SPACE))
    {
        velocity = {0, 0};
        return;
    }

    move.Normalize();

    if (CheckHitKey(KEY_INPUT_LSHIFT))
    {
        move.Mul(200);
    }
    else
    {
        move.Mul(10);
    }

    if (move.Length() > 0)
    {
        velocity.Add(move);
    }
    else
    {
        if (intersecting_)
        {
            velocity.Mul(0.99f);
        }
        else
        {
            velocity.Mul(0.999f);
        }
    }

    int x;
    int y;
    GetMousePoint(&x, &y);
    if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0)
    {
        const auto translate = world->GetCamera().transform.translate;
        transform.translate = Vec2(translate.x + static_cast<float>(x), translate.y + static_cast<float>(y));
    }
}

void PlayerComponent::Draw(DrawStack* stack)
{
    stack->Push();
    transform.ApplyDrawStack(stack);

    ImGui::Begin("Player", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::SeparatorText("Transform");
    ImGui::Text("Translate");
    ImGui::InputFloat("X", &transform.translate.x);
    ImGui::InputFloat("Y", &transform.translate.y);
    ImGui::End();

    if (const auto circleCollider = dynamic_cast<CircleCollider*>(collider.get()))
    {
        const auto& screenPos = stack->GetScreenPos();
        // 本体描画
        DrawCircleAA(screenPos.x, screenPos.y, circleCollider->radius, 16, GetColor(255, 255, 255), true);
        // コライダー描画
        DrawCircleAA(screenPos.x, screenPos.y, 1, 4, GetColor(0, 255, 0), false);
        DrawCircleAA(screenPos.x, screenPos.y, circleCollider->radius, 16, GetColor(0, 255, 0), false);

        // Gravity
        {
            auto copied = screenPos;
            copied.Add(gravityVelocity);
            DrawLineAA(screenPos.x, screenPos.y, copied.x, copied.y, GetColor(255, 255, 0), 2);
        }
        
        // Gravity + Velocity
        {
            auto copied = screenPos;
            auto vec = velocity;
            vec.Add(gravityVelocity);
            vec.Mul(1.0f);
            copied.Add(vec);
            DrawLineAA(screenPos.x, screenPos.y, copied.x, copied.y, GetColor(0, 255, 0), 2);
        }

        // Velocity
        {
            auto copied = screenPos;
            auto vec = velocity;
            vec.Mul(1.0f);
            copied.Add(vec);
            DrawLineAA(screenPos.x, screenPos.y, copied.x, copied.y, GetColor(255, 0, 0), 2);
        }

        // Normal
        {
            auto copied = screenPos;
            auto n = intersectingNormal;
            n.Mul(30.0f);
            copied.Add(n);
            DrawLineAA(screenPos.x, screenPos.y, copied.x, copied.y, GetColor(0, 0, 255), 2);
        }
    }

    stack->Pop();
}
