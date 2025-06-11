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
    shot_start_transform_ = transform;
    trail_screen_ = MakeScreen(WINDOW_WIDTH, WINDOW_HEIGHT, true);
}

PlayerComponent::~PlayerComponent()
{
    DeleteGraph(trail_screen_);
}

void PlayerComponent::Update(const float deltaTime)
{
    if (const auto circleCollider = dynamic_cast<CircleCollider*>(collider.get()))
    {
        circleCollider->radius = radius;
    }

    PhysicsComponent::Update(deltaTime);

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

    UpdateShot();
    UpdateDebugMove();
}

void PlayerComponent::UpdateMovement(const float deltaTime)
{
    const auto lastPos = transform.translate;
    PhysicsComponent::UpdateMovement(deltaTime);

    // 設置判定
    const auto& movedDistance = lastPos.Distance(transform.translate);
    can_shot_ = intersectingNormal.Length() > 0 && movedDistance < 0.3;

    should_trails_ = movedDistance > 0.3;
}

void PlayerComponent::Draw(DrawStack* stack)
{
    stack->Push();
    ApplyDrawStack(stack);

    if (world->GetType() != WorldType::StageSelect)
    {
        ImGui::Begin("Player", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::SeparatorText("Transform");
        ImGui::Text("Translate");
        ImGui::InputFloat("X", &transform.translate.x);
        ImGui::InputFloat("Y", &transform.translate.y);
        ImGui::Separator();
        ImGui::Text("Shot vec: %0.3f, %0.3f", drag_vector_.x, drag_vector_.y);
        ImGui::Text("Shot pow: %0.3f", drag_vector_.Length());
        ImGui::End();
    }

    DrawTrail(stack);

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

void PlayerComponent::Respawn()
{
    transform = shot_start_transform_;
}

Vec2 PlayerComponent::GetDragVector() const
{
    return drag_vector_;
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
        shot_start_transform_ = transform;
        auto shotVec = drag_vector_;
        shotVec.Mul(shot_power_multiplier);

        // 重力の影響を相殺する
        for (const auto& gravitySource : gravitySources)
        {
            shotVec.Add(gravitySource.Copy().Neg());
        }

        if (can_shot_)
        {
            velocity = shotVec;
        }
        drag_vector_ = {};
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
    const auto power = min((vec.Length() - shot_dead_zone) * 0.5f, max_shot_power);
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

void PlayerComponent::DrawTrail(DrawStack* stack)
{
    if (trails_.empty())
    {
        for (int i = 0; i < 20; ++i)
        {
            trails_.emplace_back(transform.translate);
        }
    }

    trails_.pop_front();
    trails_.push_back(transform.translate);

    if (!should_trails_)
    {
        return;
    }

    SetDrawScreen(trail_screen_);
    ClearDrawScreen();
    for (const auto& trailPos : trails_)
    {
        stack->Push();
        stack->Translate(transform.translate.Copy().Neg());
        stack->Translate(trailPos);
        const auto distance = transform.translate.Distance(trailPos);
        const auto& screenPos = stack->GetScreenPos();
        const auto scale = (180 - std::clamp(distance, 0.0f, 180.0f)) / 180.0f;
        DrawCircleAA(screenPos.x, screenPos.y, max(10.0f * scale, 4.0f), 16, GetColor(255, 255, 255));
        stack->Pop();
    }
    SetDrawScreen(DX_SCREEN_BACK);

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * 0.5));
    GraphFilter(trail_screen_, DX_GRAPH_FILTER_TWO_COLOR, 128, GetColor(0, 0, 0), 255, GetColor(255, 255, 255), 255);
    GraphFilter(trail_screen_, DX_GRAPH_FILTER_BRIGHT_CLIP, DX_CMP_LESS, 128, true, GetColor(0, 0, 0), 0);
    GraphFilter(trail_screen_, DX_GRAPH_FILTER_GAUSS, 32, 100);
    DrawExtendGraph(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, trail_screen_, true);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
