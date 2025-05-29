#include "PlayerComponent.h"

#include <DxLib.h>
#include <imgui.h>
#include <spdlog/spdlog.h>

#include "../Game.h"
#include "../math/CircleCollider.h"
#include "../math/Math.h"

namespace
{
    constexpr float max_shot_power = 100.0f;
    constexpr float shot_dead_zone = 20.0f;
    constexpr float shot_power_multiplier = 20.0f;
}

PlayerComponent::PlayerComponent(const int id): PhysicsComponent(id)
{
    collider = std::make_unique<CircleCollider>(radius);
    zIndex = 1100;
}

void PlayerComponent::Update(const float deltaTime)
{
    if (const auto circleCollider = dynamic_cast<CircleCollider*>(collider.get()))
    {
        circleCollider->radius = radius;
    }

    PhysicsComponent::Update(deltaTime);

    ImGui::Begin("Test");
    ImGui::Text("Last V %0.3f", velocity.Length());
    ImGui::Text("Last G %0.3f", lastGravityPower);
    if (velocity.Length() > 0)
    {
        if (velocity.Length() < 0.1)
        {
            velocity = {};
        }
        else
        {
            if (intersecting_)
            {
                // 0.5秒でとまる
                velocity.Sub(velocity.Copy().Mul(deltaTime * 2));
            }
            else
            {
                const auto multiplier = 0.99f * (1 - max(250 - lastGravityPower, 0) / 250);
                velocity.Sub(velocity.Copy().Mul(multiplier * deltaTime));
            }
        }
    }
    ImGui::End();

    UpdateShot();
    UpdateDebugMove();
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
    ImGui::Separator();
    ImGui::Text("Shot vec: %0.3f, %0.3f", drag_vector_.x, drag_vector_.y);
    ImGui::Text("Shot pow: %0.3f", drag_vector_.Length());
    ImGui::End();

    const auto& screenPos = stack->GetScreenPos();
    screen_pos_ = screenPos;

    if (const auto circleCollider = dynamic_cast<CircleCollider*>(collider.get()))
    {
        const auto& screenScale = stack->GetScreenScale();
        const auto scaledRadius = circleCollider->radius * screenScale.x;
        const auto scaledPosnum = static_cast<int>(max(16 * screenScale.x, 16));
        // 本体描画
        DrawCircleAA(screenPos.x, screenPos.y, scaledRadius, scaledPosnum, GetColor(255, 255, 255), true);
        // コライダー描画
        DrawCircleAA(screenPos.x, screenPos.y, 1, 4, GetColor(0, 255, 0), false);
        DrawCircleAA(screenPos.x, screenPos.y, scaledRadius, scaledPosnum, GetColor(0, 255, 0), false);

        // Gravity sources
        for (const auto& gravitySource : gravitySources)
        {
            auto vec = screenPos;
            vec.Add(gravitySource.Copy().Mul(0.5f));
            DrawLineAA(screenPos.x, screenPos.y, vec.x, vec.y, GetColor(0, 255, 0), 2);
        }

        // Normal
        {
            auto copied = screenPos;
            auto n = intersectingNormal;
            n.Mul(30.0f);
            copied.Add(n);
            DrawLineAA(screenPos.x, screenPos.y, copied.x, copied.y, GetColor(0, 0, 255), 2);
        }

        // Shot vec
        if (isDragging && drag_vector_.Length() > 0)
        {
            const auto offset = radius * 1.5f;
            auto vec = drag_vector_;
            const auto arrowStart = screenPos.Copy().Add(vec.Normalized().Mul(offset));
            auto copied = arrowStart;
            const auto blue = vec.Length() / max_shot_power * 255;
            copied.Add(vec.Copy().Normalize().Mul(offset));
            vec.Mul(2);
            copied.Add(vec);

            const auto color = GetColor(255, 0, static_cast<int>(blue));

            DrawLineAA(arrowStart.x, arrowStart.y, copied.x, copied.y, color, 2);

            const auto leftArrowDir = vec.Copy().Neg().Rotate(30 * Math::deg_to_rad).Normalize();
            const auto leftArrowPos = copied.Copy().Add(leftArrowDir.Copy().Mul(16));
            DrawLineAA(copied.x, copied.y, leftArrowPos.x, leftArrowPos.y, color, 2);

            const auto rightArrowDir = vec.Copy().Neg().Rotate(-30 * Math::deg_to_rad).Normalize();
            const auto rightArrowPos = copied.Copy().Add(rightArrowDir.Copy().Mul(16));
            DrawLineAA(copied.x, copied.y, rightArrowPos.x, rightArrowPos.y, color, 2);
        }
    }

    stack->Pop();
}

void PlayerComponent::UpdateShot()
{
    int x;
    int y;

    GetMousePoint(&x, &y);

    const auto clicking = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;
    if (!isDragging && clicking)
    {
        if (
            const auto distance = screen_pos_.Distance({static_cast<float>(x), static_cast<float>(y)});
            distance > radius + 16
        )
        {
            return;
        }
        isDragging = true;
    }

    if (isDragging && !clicking)
    {
        auto shotVec = drag_vector_;
        shotVec.Mul(shot_power_multiplier);

        // 重力の影響を相殺する
        for (const auto & gravitySource : gravitySources)
        {
            shotVec.Add(gravitySource.Copy().Neg());
        }
        
        velocity = shotVec;
        isDragging = false;
    }

    if (!isDragging)
    {
        return;
    }

    x -= 640; // FIXME マジックナンバーをやめる
    y -= 360; // FIXME マジックナンバーをやめる

    const auto cameraTranslate = world->GetCamera().transform.translate;
    const auto zoom = world->GetCamera().zoom;
    const auto dx = static_cast<float>(x) / zoom;
    const auto dy = static_cast<float>(y) / zoom;

    auto vec = transform.translate.Copy().Sub({cameraTranslate.x + dx, cameraTranslate.y + dy});
    if (vec.Length() < shot_dead_zone)
    {
        drag_vector_ = {};
        return;
    }
    const auto power = min(vec.Length() - shot_dead_zone, max_shot_power);
    drag_vector_ = vec.Normalize().Mul(power);
}

void PlayerComponent::UpdateDebugMove()
{
    int x;
    int y;

    GetMousePoint(&x, &y);

    x -= 640; // FIXME マジックナンバーをやめる
    y -= 360; // FIXME マジックナンバーをやめる

    if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0 && CheckHitKey(KEY_INPUT_LSHIFT) != 0)
    {
        auto translate = world->GetCamera().transform.translate;
        velocity = {0, 0};

        const auto zoom = world->GetCamera().zoom;
        Vec2 delta = {static_cast<float>(x), static_cast<float>(y)};
        delta.Div(zoom);

        transform.translate = translate.Add(delta);
    }
}
