#include "PlayWorld.h"

#include <imgui.h>
#include <memory>
#include <spdlog/spdlog.h>

#include "EditorWorld.h"
#include "../Game.h"
#include "../component/GoalHoleComponent.h"
#include "../component/misc/GoalCompassComponent.h"
#include "../component/planet/CommonPlanetComponent.h"
#include "../editor/StageFileManager.h"
#include "../graph/Graphs.h"
#include "../math/BoundingBox.h"
#include "../math/Math.h"

PlayWorld::PlayWorld(std::string id) : id_(std::move(id))
{
    stageDefine = *StageFileManager::GetOrLoadDefine(id);
}

PlayWorld::PlayWorld(std::string id, const StageDefine& stageDefine, const bool debug)
    : id_(std::move(id)), stageDefine(stageDefine), debug_(debug)
{
}

void PlayWorld::Update(const float& deltaTime)
{
    StageWorld::Update(deltaTime);
    UpdateCamera(deltaTime);
    CheckPlayableArea();

    if (debug_ && !Game::uiHidden)
    {
        ImGui::Begin("Debug");
        ImGui::Checkbox("Free camera", &free_camera_);
        if (ImGui::Button("Return editor"))
        {
            Game::instance->ChangeWorldWithTransition<EditorWorld>(TransitionMode::Circle, id_, stageDefine);
        }
        ImGui::End();
    }
}

void PlayWorld::CheckPlayableArea() const
{
    // プレイアブルエリアが設定されていない場合は何もしない
    if (stageDefine.playableAreas.empty())
    {
        return;
    }

    const auto areaDefine = stageDefine.playableAreas[0]; // vectorで扱っているがとりあえず一つだけ
    const auto origin = Math::GetCenter(areaDefine.start, areaDefine.end);
    const auto width = std::abs(areaDefine.start.x - areaDefine.end.x);
    const auto height = std::abs(areaDefine.start.y - areaDefine.end.y);
    const auto& area = BoundingBox(width, height);

    // // スタート地点がプレイアブルエリアに含まれていない場合は何もしない
    // if (!area.Contains(origin, stageDefine.startPos))
    // {
    //     return;
    // }

    // プレイヤーがプレイアブルエリア内にいる場合は何もしない
    if (area.Contains(origin, GetPlayer()->transform.translate))
    {
        return;
    }

    // Reload();
    // TODO
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
        component->isSatellite = planet.isSatellite;
        component->rotationSpeed = planet.rotationSpeed;
        component->rotationOriginOffset = {planet.rotationOriginOffsetX, planet.rotationOriginOffsetY};
        AddComponent(component);
    }

    const auto goalHole = std::make_shared<GoalHoleComponent>(NextComponentId());
    goalHole->transform.translate = stageDefine.goal.pos;
    goalHole->transform.rotation = stageDefine.goal.rot;
    AddComponent(goalHole);

    const auto compass = std::make_shared<GoalCompassComponent>(NextComponentId());
    compass->targetId = goalHole->GetId();
    AddComponent(compass);

    GetPlayer()->transform.translate = stageDefine.startPos;

    GetCamera().zoom = 0.8f;
}

void PlayWorld::Reload() const
{
    if (debug_)
    {
        auto playStage = std::make_unique<PlayWorld>(id_, stageDefine, true);
        Game::instance->ChangeWorldWithTransition(TransitionMode::Slide, std::move(playStage));
        return;
    }
    StageWorld::Reload();
}

void PlayWorld::DrawBackground(DrawStack& stack)
{
    Graphs::stageBackground->Draw(0, 0);
}

void PlayWorld::UpdateCamera(float deltaTime)
{
    if (free_camera_)
    {
        return;
    }

    auto& camera = GetCamera();
    const auto player = GetPlayer();
    const auto focusPos = player->transform.translate;
    camera.transform.translate = Math::Lerp(camera.transform.translate, focusPos, deltaTime * 3.0f);
}

WorldType PlayWorld::GetType() const
{
    return WorldType::Play;
}

std::string PlayWorld::GetStageId() const
{
    return id_;
}

void PlayWorld::OnCameraMoveWithMouse(CameraComponent* camera)
{
    StageWorld::OnCameraMoveWithMouse(camera);

    free_camera_ = true;
}

void PlayWorld::OnShot()
{
    free_camera_ = false;
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
