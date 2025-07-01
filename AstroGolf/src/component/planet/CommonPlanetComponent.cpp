#include "CommonPlanetComponent.h"

#include "../../Game.h"
#include "../../editor/PlanetGraphs.h"

CommonPlanetComponent::CommonPlanetComponent(const int id, const float radius, std::string graphId):
    PlanetComponent(id, radius),
    graphId(std::move(graphId))
{
}

void CommonPlanetComponent::Update(float deltaTime)
{
    planetGravity = 9.8f * radius * gravityMultiplier;
    PlanetComponent::Update(deltaTime);
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
    }

    stack->Pop();
}
