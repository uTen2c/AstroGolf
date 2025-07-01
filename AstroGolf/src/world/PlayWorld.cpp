#include "PlayWorld.h"

#include <imgui.h>
#include <memory>

#include "EditorWorld.h"
#include "../Game.h"
#include "../component/GoalHoleComponent.h"
#include "../component/planet/CommonPlanetComponent.h"

PlayWorld::PlayWorld(std::string id, const StageDefine& stageDefine, const bool debug)
    : id_(std::move(id)), stageDefine(stageDefine), debug_(debug)
{
}

void PlayWorld::Update(const float& deltaTime)
{
    StageWorld::Update(deltaTime);

    if (debug_)
    {
        ImGui::Begin("Debug");
        if (ImGui::Button("Stop"))
        {
            Game::instance->ChangeWorldWithTransition<EditorWorld>(TransitionMode::Circle, id_, stageDefine);
        }
        ImGui::End();
    }
}

void PlayWorld::Init()
{
    StageWorld::Init();

    // ReSharper disable once CppUseStructuredBinding
    for (const auto& planet : stageDefine.planets)
    {
        const auto component = std::make_shared<
            CommonPlanetComponent>(NextComponentId(), planet.radius, planet.graphId);
        component->transform.translate = planet.pos;
        component->gravityMultiplier = planet.gravityMultiplier;
        AddComponent(component);
    }

    const auto goalHole = std::make_shared<GoalHoleComponent>(NextComponentId());
    goalHole->transform.translate = stageDefine.goal.pos;
    goalHole->transform.rotation = stageDefine.goal.rot;
    AddComponent(goalHole);

    GetPlayer()->transform.translate = stageDefine.startPos;
}

WorldType PlayWorld::GetType() const
{
    return WorldType::Play;
}

std::string PlayWorld::GetStageId() const
{
    return id_;
}

void PlayWorld::OnGoal()
{
    if (debug_)
    {
        Game::instance->ChangeWorldWithTransition<EditorWorld>(TransitionMode::Circle, id_, stageDefine);
        return;
    }
    StageWorld::OnGoal();
}
