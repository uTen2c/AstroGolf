#include "CommonPlanetComponent.h"

#include "../../Game.h"
#include "../../editor/PlanetGraphs.h"

CommonPlanetComponent::CommonPlanetComponent(const int id, const float radius, std::string graphId):
    PlanetComponent(id, radius),
    graphId(std::move(graphId))
{
}

void CommonPlanetComponent::Update(const float deltaTime)
{
    planetGravity = 9.8f * radius * gravityMultiplier;
    PlanetComponent::Update(deltaTime);

    if (Game::instance->GetWorld().GetType() == WorldType::Editor)
    {
        UpdateInEditor(deltaTime);
    }
    else
    {
        UpdateInPlay(deltaTime);
    }
}

void CommonPlanetComponent::UpdateInPlay(float deltaTime)
{
    UpdateSatellite(deltaTime);
}

void CommonPlanetComponent::UpdateSatellite(float deltaTime)
{
    if (!isSatellite)
    {
        return;
    }

    // 最初に一度だけ回転に必要な情報を取る
    if (!origin_world_pos_initialized_)
    {
        origin_world_pos_ = transform.translate + rotationOriginOffset;
        rotation_distance_ = rotationOriginOffset.Length();
        rotation_angle_ = origin_world_pos_.SignedAngle(transform.translate);
        origin_world_pos_initialized_ = true;
    }

    rotation_angle_ += rotationSpeed * deltaTime;
    const auto offset = Vec2(rotation_distance_, 0.0f).Rotate(rotation_angle_);
    transform.translate = origin_world_pos_ + offset;
}

void CommonPlanetComponent::UpdateInEditor(float deltaTime)
{
    if (rotation_origin_dragging_)
    {
        const auto mousePos = Game::Device().MousePos();
        const auto zoom = world->GetCamera().zoom;
        const auto moved = mousePos - rotation_origin_drag_start_mouse_pos_;
        rotationOriginOffset = rotation_origin_drag_start_offset_ + moved / zoom;
    }
}

void CommonPlanetComponent::Draw(DrawStack* stack)
{
    stack->Push();
    transform.ApplyDrawStack(stack);

    if (const auto& graph = PlanetGraphs::GetGraph(graphId))
    {
        graph->Draw(*stack);
    }

    if (Game::debugEnabled || Game::instance->GetWorld().GetType() == WorldType::Editor)
    {
        const auto pos = stack->GetScreenPos();
        const auto scale = stack->GetScreenScale();
        const auto posnum = static_cast<int>(max(32 * scale.x, 32));
        DrawCircleAA(pos.x, pos.y, radius * scale.x, posnum, GetColor(0, 255, 0), false);

        if (isSatellite)
        {
            stack->Push();
            stack->Translate(rotationOriginOffset);
            const auto& originPos = stack->GetScreenPos();
            const auto originColor = rotation_origin_dragging_ ? GetColor(63, 63, 255) : GetColor(255, 255, 255);
            DrawCircleAA(originPos.x, originPos.y, 4, 8, originColor, true);
            DrawLineAA(pos.x, pos.y, originPos.x, originPos.y, GetColor(0, 0, 255));

            const auto r = rotationOriginOffset.Length() * Game::instance->GetWorld().GetCamera().zoom;
            DrawCircleAA(originPos.x, originPos.y, r, 32, GetColor(127, 127, 127), false);
            stack->Pop();

            if (Game::Device().LeftClicked())
            {
                const auto& mousePos = Game::Device().MousePos();
                if (mousePos.Distance(originPos) < 8)
                {
                    rotation_origin_dragging_ = true;
                    rotation_origin_drag_start_mouse_pos_ = mousePos;
                    rotation_origin_drag_start_offset_ = rotationOriginOffset;
                    if (world)
                    {
                        world->GetCamera().draggable = false;
                    }
                }
            }

            if (Game::Device().LeftReleased() && rotation_origin_dragging_)
            {
                rotation_origin_dragging_ = false;
                if (world)
                {
                    world->GetCamera().draggable = true;
                }
            }
        }
    }

    stack->Pop();
}
