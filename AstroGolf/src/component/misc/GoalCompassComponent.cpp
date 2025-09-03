#include "GoalCompassComponent.h"

#include <spdlog/spdlog.h>

#include "../../Game.h"
#include "../../graph/Graphs.h"
#include "../../math/collider/BoxCollider.h"
#include "../../world/World.h"
#include "../../math/CrossPointResult.h"
#include "../../math/Math.h"

GoalCompassComponent::GoalCompassComponent(const int id): Component(id)
{
}

void GoalCompassComponent::Update(float delta)
{
    if (targetId == -1)
    {
        return;
    }
    const auto& target = world->GetComponent(targetId);
    if (!target)
    {
        return;
    }

    const auto& center = world->GetCamera().transform.translate;
    transform.translate = center;

    const auto& camera = world->GetCamera();
    const auto scaledWidth = WINDOW_WIDTH / camera.zoom;
    const auto scaledHeight = WINDOW_HEIGHT / camera.zoom;
    BoxCollider fullscreenBox(scaledWidth + 128, scaledHeight + 128);

    // 画面内にゴールが存在する
    if (fullscreenBox.Contains(center, target->transform.translate))
    {
        hidden_ = true;
        return;
    }

    BoxCollider box(scaledWidth - 128, scaledHeight - 128);

    const Vec2 edges[4][2] = {
        {center + Vec2(box.GetLeft(), box.GetTop()), center + Vec2(box.GetRight(), box.GetTop())}, // Top
        {center + Vec2(box.GetLeft(), box.GetBottom()), center + Vec2(box.GetRight(), box.GetBottom())}, // Bottom
        {center + Vec2(box.GetLeft(), box.GetTop()), center + Vec2(box.GetLeft(), box.GetBottom())}, // Left
        {center + Vec2(box.GetRight(), box.GetTop()), center + Vec2(box.GetRight(), box.GetBottom())}, // Right
    };

    bool shouldShowFlag = false;
    Vec2 flagPos;

    for (const auto& [edgeStart, edgeEnd] : edges)
    {
        const auto& result = Math::GetCrossPoint(center, target->transform.translate, edgeStart, edgeEnd);
        if (result.crossed)
        {
            shouldShowFlag = true;
            flagPos = result.point;
            compass_angle_ = (flagPos - target->transform.translate).Normalized().SignedAngle({-1, 0});
            break;
        }
    }

    if (shouldShowFlag)
    {
        transform.translate = flagPos;
        hidden_ = false;
    }
    else
    {
        hidden_ = true;
    }
}

void GoalCompassComponent::Draw(DrawStack* stack)
{
    if (hidden_)
    {
        return;
    }
    stack->Push();
    ApplyDrawStack(stack);

    Graphs::goalFlag->Draw(*stack);

    stack->Push();
    stack->Rotate(compass_angle_);
    Graphs::goalCompass->Draw(*stack);

    stack->Pop();
    stack->Pop();
}
