#include "EditorWorld.h"

#include <fstream>
#include <imgui.h>
#include <memory>
#include <spdlog/spdlog.h>

#include "../Game.h"
#include "../component/planet/CommonPlanetComponent.h"
#include "../editor/PlanetGraphs.h"
#include "../editor/StageDefine.h"
// #include "../ImGuiExtension.h"

EditorWorld::EditorWorld()
{
}

EditorWorld::~EditorWorld()
{
    Game::debugEnabled = false;
}

void EditorWorld::Init()
{
    World::Init();
    Game::debugEnabled = true;
}

void EditorWorld::Draw()
{
    World::Draw();
}

void EditorWorld::DrawBackground(DrawStack& stack)
{
    World::DrawBackground(stack);
    DrawGrid(stack);
    DrawPreview(stack);

    ImGui::Begin("Stage Editor");
    if (ImGui::Button("Load"))
    {
        Load("demo");
    }
    ImGui::End();
}

void EditorWorld::Update(const float& deltaTime)
{
    World::Update(deltaTime);

    UpdatePlanetEditor(deltaTime);

    GetPlayer()->transform.translate = {10000, 10000};
}

void EditorWorld::UpdatePlanetEditor(const float& deltaTime)
{
    const auto mousePos = GetMousePos();
    const auto worldPos = GetMouseWorldPos();

    if (Game::Device().LeftClicked())
    {
        clicked_pos_ = mousePos;
        return;
    }

    if (!Game::Device().LeftReleased() || mousePos.Distance(clicked_pos_) > 10)
    {
        return;
    }

    const auto id = NextComponentId();
    definedComponentIds.emplace_back(id);

    const auto component = std::make_shared<CommonPlanetComponent>(id, 350 / 2, "purple");
    component->transform.translate = worldPos;
    AddComponent(component);

    spdlog::info("Clicked {}, {}", worldPos.x, worldPos.y);
}

WorldType EditorWorld::GetType() const
{
    return WorldType::Editor;
}

void EditorWorld::Load(const std::string& stageId)
{
    const auto& filename = fmt::format("assets/stage/{}.json", stageId);
    std::ifstream file(filename);
    if (!file.is_open())
    {
        spdlog::error("Failed to open {}", filename);
        return;
    }

    json json;
    file >> json;

    for (const auto id : definedComponentIds)
    {
        RemoveComponent(id);
    }
    definedComponentIds.clear();

    stage_id_ = stageId;
    for (const auto& componentJson : json["components"])
    {
    }
}

void EditorWorld::DrawPreview(DrawStack& stack)
{
    const auto& worldPos = GetMouseWorldPos();

    const auto component = std::make_shared<CommonPlanetComponent>(-1, 350 / 2, "purple");
    component->transform.translate = worldPos;

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * 0.5));
    component->Draw(&stack);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

Vec2 EditorWorld::GetMouseWorldPos() const
{
    const auto& mousePos = GetMousePos();
    auto worldPos = GetWorldPos(mousePos);

    // Ctrlを押している間はスナップする
    if (Game::Device().ctrlKey.Pressing())
    {
        worldPos = Vec2(round(worldPos.x / 25) * 25, round(worldPos.y / 25) * 25);
    }

    return worldPos;
}

void EditorWorld::DrawGrid(DrawStack& stack)
{
    stack.Push();

    const auto& pos = stack.GetScreenPos();
    const auto& scale = stack.GetScreenScale();

    const auto gridSize = 100 * scale.x;
    const auto offset = static_cast<int>(ceil(abs(pos.x) / scale.x / gridSize));
    const auto count = static_cast<int>(ceil(WINDOW_WIDTH / 100 / scale.x)) * 3;

    // グリッド表示
    for (int i = -offset - count; i < offset + count; ++i)
    {
        const auto gridSizeX = gridSize * i;
        DrawLine(-2000, pos.y + gridSizeX, 2000, pos.y + gridSizeX, GetColor(64, 64, 64));
        DrawLine(pos.x + gridSizeX, -2000, pos.x + gridSizeX, 2000, GetColor(64, 64, 64));
    }

    // X, Yの十字線描画
    DrawLine(-2000, pos.y, 2000, pos.y, GetColor(255, 0, 0));
    DrawLine(pos.x, -2000, pos.x, 2000, GetColor(0, 255, 0));
    stack.Pop();
}

Vec2 EditorWorld::GetMousePos()
{
    int mouseX;
    int mouseY;
    GetMousePoint(&mouseX, &mouseY);
    return {static_cast<float>(mouseX), static_cast<float>(mouseY)};
}
