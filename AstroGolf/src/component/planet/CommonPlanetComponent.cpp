#include "CommonPlanetComponent.h"

#include "../../Game.h"
#include "../../editor/PlanetGraphs.h"

CommonPlanetComponent::CommonPlanetComponent(const int id, const float radius, std::string graphId):
    PlanetComponent(id, radius),
    graph_id_(std::move(graphId))
{
}

void CommonPlanetComponent::Update(float deltaTime)
{
    PlanetComponent::Update(deltaTime);
}

void CommonPlanetComponent::Draw(DrawStack* stack)
{
    stack->Push();
    transform.ApplyDrawStack(stack);

    if (const auto& graph = PlanetGraphs::GetGraph(graph_id_))
    {
        graph->Draw(*stack);
    }

    if (Game::debugEnabled)
    {
        const auto pos = stack->GetScreenPos();
        const auto scale = stack->GetScreenScale();
        const auto posnum = static_cast<int>(max(32 * scale.x, 32));
        DrawCircleAA(pos.x, pos.y, radius * scale.x, posnum, GetColor(0, 255, 0), false);
    }

    stack->Pop();
}
