#include "BallisticComponent.h"

#include <algorithm>
#include <DxLib.h>

#include "../../Game.h"

BallisticComponent::BallisticComponent(const int id): PhysicsComponent(id)
{
    collider = std::make_unique<CircleCollider>(12.0f);
    trail_screen_ = MakeScreen(WINDOW_WIDTH, WINDOW_HEIGHT, true);
}

BallisticComponent::~BallisticComponent()
{
    DeleteGraph(trail_screen_);
}

void BallisticComponent::Draw(DrawStack* stack)
{
}

void BallisticComponent::DrawPoints(DrawStack* stack) const
{
    if (!shouldDraw)
    {
        return;
    }

    stack->Push();
    ApplyDrawStack(stack);

    SetDrawScreen(trail_screen_);
    ClearDrawScreen();

    stack->Push();
    stack->Translate(transform.translate.Copy().Neg());
    
    for (const auto& trailPos : trails_)
    {
        stack->Push();
        stack->Translate(trailPos);
        const auto& screenPos = stack->GetScreenPos();
        DrawCircleAA(screenPos.x, screenPos.y, 3, 16, GetColor(255, 255, 255));
        stack->Pop();
    }
    
    stack->Pop();

    SetDrawScreen(DX_SCREEN_BACK);

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * 0.5));
    // GraphFilter(trail_screen_, DX_GRAPH_FILTER_TWO_COLOR, 128, GetColor(0, 0, 0), 255, GetColor(255, 255, 255), 255);
    // GraphFilter(trail_screen_, DX_GRAPH_FILTER_BRIGHT_CLIP, DX_CMP_LESS, 128, true, GetColor(0, 0, 0), 0);
    // GraphFilter(trail_screen_, DX_GRAPH_FILTER_GAUSS, 32, 100);
    DrawExtendGraph(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, trail_screen_, true);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    stack->Pop();
}

void BallisticComponent::Update(float deltaTime)
{
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
                const auto multiplier = 0.99f * (1 - max(250 - last_gravity_power, 0) / 250);
                velocity.Sub(velocity.Copy().Mul(multiplier * deltaTime));
            }
        }
    }
}

void BallisticComponent::CreatePoint()
{
    trails_.push_back(transform.translate);
}

void BallisticComponent::Reset()
{
    trails_.clear();
}
