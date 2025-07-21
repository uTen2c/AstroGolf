#include "EditorWorld.h"

#include <fstream>
#include <imgui.h>
#include <memory>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#include "PlayWorld.h"
#include "../Game.h"
#include "../component/editor/PlayerStartAnchorComponent.h"
#include "../component/planet/CommonPlanetComponent.h"
#include "../editor/PlanetGraphs.h"
#include "../editor/StageDefine.h"
#include "../ImEx.h"
#include "../editor/StageFileManager.h"
#include "../game/StageManager.h"
#include "../math/Math.h"

using json = nlohmann::json;

EditorWorld::EditorWorld()
{
    menuEnabled = false;
}

EditorWorld::EditorWorld(const std::string& id, const StageDefine& stageDefine)
    : EditorWorld()
{
    has_define_ = true;
    stage_id_ = id;
    stage_define_ = stageDefine;
}

EditorWorld::~EditorWorld() = default;

void EditorWorld::Init()
{
    World::Init();

    GetCamera().minZoomLevel = 0.1f;

    if (!has_define_)
    {
        return;
    }
    Load(stage_id_);
}

void EditorWorld::Draw()
{
    if (place_type_ == PlaceType::PlayableArea)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * 0.4));
    }
    World::Draw();
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void EditorWorld::DrawBackground(DrawStack& stack)
{
    World::DrawBackground(stack);
    DrawGrid(stack);
}

void EditorWorld::PostDraw(DrawStack& stack)
{
    DrawPreview(stack);
    DrawGizmo(stack);
    DrawPlayableArea();
    DrawMousePos();
}

void EditorWorld::Update(const float& deltaTime)
{
    World::Update(deltaTime);

    UpdatePlanetEditor(deltaTime);
    UpdatePlayableAreaEditor();
    UpdateComponentIndicator();
    UpdateMovement();
    UpdateDelete();

    PushStyles();
    UpdateUi();
    PopStyles();

    GetPlayer()->transform.translate = GetCamera().transform.translate + Vec2(10000, 10000);
}

void EditorWorld::UpdatePlanetEditor(const float& deltaTime)
{
    const auto mousePos = GetMousePos();
    const auto worldPos = GetMouseWorldPos();

    const auto& io = ImGui::GetIO();

    // 右クリックで選択解除
    if (Game::Device().RightClicked() && !io.WantCaptureMouse)
    {
        selected_component_id_ = -1;
        return;
    }

    if (Game::Device().LeftClicked() && !io.WantCaptureMouse)
    {
        clicked_pos_ = mousePos;
        return;
    }

    // ステージを読み込んでいなかったらなにもしない
    if (stage_id_.empty())
    {
        return;
    }

    // 設置モードがプレイアブルエリアの場合は何もしない
    if (place_type_ == PlaceType::PlayableArea)
    {
        return;
    }

    if (!Game::Device().LeftReleased() || mousePos.Distance(clicked_pos_) > 10)
    {
        return;
    }

    if (hovering_component_id_ != -1)
    {
        selected_component_id_ = hovering_component_id_;
        return;
    }

    const auto id = NextComponentId();
    defined_component_ids_.emplace_back(id);

    switch (place_type_)
    {
    case PlaceType::PlayerStart:
        PlacePlayerStartAnchor(worldPos);
        break;
    case PlaceType::GoalHole:
        PlaceGoalHole(worldPos);
        break;
    case PlaceType::Component:
        {
            const auto component = std::make_shared<CommonPlanetComponent>(id, planet_radius_, planet_graph_id_);
            component->transform.translate = worldPos;
            AddComponent(component);
            break;
        }
    case PlaceType::PlayableArea:
        break;
    }

    selected_component_id_ = id;
    spdlog::info("Clicked {}, {}", worldPos.x, worldPos.y);
}

void EditorWorld::UpdatePlayableAreaEditor()
{
    if (place_type_ != PlaceType::PlayableArea)
    {
        return;
    }

    const auto& worldPos = GetMouseWorldPos();
    if (!is_area_dragging_ && Game::Device().RightClicked())
    {
        area_start_pos_ = worldPos;
        is_area_dragging_ = true;
    }

    if (is_area_dragging_ && Game::Device().RightReleased())
    {
        is_area_dragging_ = false;
    }

    if (is_area_dragging_ && Game::Device().RightClicking())
    {
        area_end_pos_ = worldPos;
    }
}

void EditorWorld::PlacePlayerStartAnchor(const Vec2& pos)
{
    if (!startAnchor)
    {
        startAnchor = std::make_shared<PlayerStartAnchorComponent>(NextComponentId());
        startAnchor->transform.translate = pos;
        AddComponent(startAnchor);
    }

    startAnchor->transform.translate = pos;
}

void EditorWorld::PlaceGoalHole(const Vec2& pos)
{
    if (!goalHole)
    {
        goalHole = std::make_shared<GoalHoleComponent>(NextComponentId());
        goalHole->transform.translate = pos;
        AddComponent(goalHole);
    }

    goalHole->transform.translate = pos;
}

void EditorWorld::UpdateMovement()
{
    const auto& component = GetComponent(selected_component_id_);
    if (!component)
    {
        GetCamera().draggable = true;
        movement_type_ = MovementType::None;
        return;
    }

    const auto scale = GetCamera().zoom;
    const auto& pos = component->transform.translate;
    const auto& mousePos = GetMouseWorldPos();
    const auto padding = 16 / scale;
    const auto max = 64 / scale;
    const auto& relativePos = mousePos - pos;
    if (Game::Device().LeftClicked() && movement_type_ == MovementType::None)
    {
        // 真ん中ドラッグ
        if (abs(relativePos.x) <= padding && abs(relativePos.y) <= padding)
        {
            GetCamera().draggable = false;
            movement_start_component_pos_ = component->transform.translate;
            movement_start_mouse_pos_ = mousePos;
            movement_type_ = MovementType::Both;
        }
        // X軸ドラッグ
        else if (abs(relativePos.y) <= padding && relativePos.x > padding && relativePos.x < max)
        {
            GetCamera().draggable = false;
            movement_start_component_pos_ = component->transform.translate;
            movement_start_mouse_pos_ = mousePos;
            movement_type_ = MovementType::X;
        }
        // Y軸ドラッグ
        else if (abs(relativePos.x) <= padding && relativePos.y > padding && relativePos.y < max)
        {
            GetCamera().draggable = false;
            movement_start_component_pos_ = component->transform.translate;
            movement_start_mouse_pos_ = mousePos;
            movement_type_ = MovementType::Y;
        }
    }

    if (!Game::Device().LeftClicking() && movement_type_ != MovementType::None)
    {
        GetCamera().draggable = true;
        movement_type_ = MovementType::None;
    }

    if (movement_type_ == MovementType::None)
    {
        return;
    }

    if (Game::Device().menuKey.Pressed())
    {
        component->transform.translate = movement_start_component_pos_;
        movement_type_ = MovementType::None;
        return;
    }

    Vec2 movement;
    const Vec2 moved = mousePos - movement_start_mouse_pos_;
    if (movement_type_ == MovementType::Both || movement_type_ == MovementType::X)
    {
        movement.x = moved.x;
    }

    if (movement_type_ == MovementType::Both || movement_type_ == MovementType::Y)
    {
        movement.y = moved.y;
    }

    component->transform.translate = movement_start_component_pos_ + movement;
}

void EditorWorld::UpdateUi()
{
    ImGui::SetNextWindowSize(ImVec2(360, WINDOW_HEIGHT), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(WINDOW_WIDTH - 360, 0), ImGuiCond_Always);
    ImGui::Begin("ステージエディター", nullptr,
                 ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::MenuItem("新規作成"))
        {
            new_stage_id_ = "";
            ImGui::OpenPopup("ステージ新規作成");
        }

        if (ImGui::MenuItem("読み込み"))
        {
            ImGui::OpenPopup("ステージ読み込み");
        }

        ImGui::BeginDisabled(!startAnchor || !goalHole);
        if (ImGui::MenuItem("保存"))
        {
            Save();
        }
        if (ImGui::MenuItem("テストプレイ"))
        {
            Save();
            auto playStage = std::make_unique<PlayWorld>(stage_id_, GetDefine(), true);
            Game::instance->ChangeWorldWithTransition(TransitionMode::Slide, std::move(playStage));
        }
        ImGui::EndDisabled();

        if (ImGui::BeginPopupModal("ステージ新規作成", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("ステージIDを入力してください");

            ImEx::InputText("ID", &new_stage_id_);

            if (ImGui::Button("キャンセル", ImVec2(140, 0)))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();

            ImGui::BeginDisabled(!CanCreateStage(new_stage_id_));
            if (ImGui::Button("作成", ImVec2(140, 0)))
            {
                CreateStage(new_stage_id_);
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndDisabled();
            ImGui::EndPopup();
        }

        if (ImGui::BeginPopupModal("ステージ読み込み", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("読み込むステージを選択してください");

            auto stageId = !stage_id_.empty() ? stage_id_ : "##";
            const auto& stageIds = StageManager::GetStageIds();
            if (ImEx::Combo("Stage id", stageId, stageIds))
            {
                Load(stageId);
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::Button("キャンセル", ImVec2(120, 0)))
            {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        ImGui::EndMenuBar();
    }

    DrawGeneralEditor();

    if (stage_id_.empty())
    {
        ImGui::End();
        return;
    }

    const auto& component = GetComponent(selected_component_id_);
    if (!component)
    {
        ImGui::Text("クリックしてコンポーネントを\n選択してください");
        ImGui::End();
        return;
    }

    if (ImGui::TreeNodeEx("位置", ImGuiTreeNodeFlags_DefaultOpen))
    {
        // 座標
        ImGui::InputFloat("X", &component->transform.translate.x);
        ImGui::InputFloat("Y", &component->transform.translate.y);

        // 回転（GoalHoleのみ）
        if (const auto goal = dynamic_cast<GoalHoleComponent*>(component))
        {
            ImGui::SliderAngle("Rot", &goal->transform.rotation);
        }

        ImGui::TreePop();
    }

    if (const auto planet = dynamic_cast<CommonPlanetComponent*>(component))
    {
        if (ImGui::TreeNodeEx("詳細 (惑星)", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::SliderFloat("半径", &planet->radius, 4, 600);
            if (ImEx::Combo("タイプ", planet->graphId, PlanetGraphs::GetGraphIds()))
            {
                planet->radius = PlanetGraphs::GetGraphInfo(planet->graphId).radius;
            }

            ImGui::InputFloat("重力倍率", &planet->gravityMultiplier);

            ImGui::Checkbox("衛星", &planet->isSatellite);

            if (planet->isSatellite)
            {
                ImGui::SliderAngle("回転角度/秒", &planet->rotationSpeed);
            }

            ImGui::TreePop();
        }
    }

    ImGui::End();
}

void EditorWorld::UpdateDelete()
{
    const auto& component = GetComponent(selected_component_id_);
    if (!component)
    {
        return;
    }

    // スタートアンカーとゴールは消せない
    if (
        startAnchor && selected_component_id_ == startAnchor->GetId()
        || goalHole && selected_component_id_ == goalHole->GetId())
    {
        return;
    }

    if (Game::Device().deleteKey.Pressed())
    {
        RemoveComponent(component->GetId());
    }
}

void EditorWorld::DrawGeneralEditor()
{
    if (stage_id_.empty())
    {
        ImGui::Text("「読み込む」からステージデータを\n読み込んでください");
        return;
    }

    ImGui::Text(std::format("ステージID: {}", stage_id_).c_str());
    if (ImGui::TreeNodeEx("設置モード", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::Button("スタート地点"))
        {
            place_type_ = PlaceType::PlayerStart;
        }
        ImGui::SameLine();
        if (ImGui::Button("ゴール"))
        {
            place_type_ = PlaceType::GoalHole;
        }
        ImGui::SameLine();
        if (ImGui::Button("惑星"))
        {
            place_type_ = PlaceType::Component;
        }
        ImGui::SameLine();
        if (ImGui::Button("プレイエリア"))
        {
            place_type_ = PlaceType::PlayableArea;
            selected_component_id_ = -1;
        }
        if (ImEx::Combo("惑星タイプ", planet_graph_id_, PlanetGraphs::GetGraphIds()))
        {
            planet_radius_ = PlanetGraphs::GetGraphInfo(planet_graph_id_).radius;
        }

        ImGui::TreePop();
    }
}

WorldType EditorWorld::GetType() const
{
    return WorldType::Editor;
}

void EditorWorld::Load(const std::string& stageId)
{
    const auto& define = *StageFileManager::LoadDefine(stageId);
    stage_define_ = define;

    for (const auto id : defined_component_ids_)
    {
        RemoveComponent(id);
    }
    defined_component_ids_.clear();

    if (startAnchor)
    {
        RemoveComponent(startAnchor->GetId());
        startAnchor = nullptr;
    }

    if (goalHole)
    {
        RemoveComponent(goalHole->GetId());
        goalHole = nullptr;
    }

    stage_id_ = stageId;

    // ReSharper disable once CppUseStructuredBinding
    for (const auto& planet : define.planets)
    {
        const auto id = NextComponentId();
        const auto component = std::make_shared<
            CommonPlanetComponent>(id, planet.radius, planet.graphId);
        component->transform.translate = planet.pos;
        component->gravityMultiplier = planet.gravityMultiplier;
        component->isSatellite = planet.isSatellite;
        component->rotationSpeed = planet.rotationSpeed;
        component->rotationOriginOffset = {planet.rotationOriginOffsetX, planet.rotationOriginOffsetY};
        defined_component_ids_.emplace_back(id);
        AddComponent(component);
    }

    goalHole = std::make_shared<GoalHoleComponent>(NextComponentId());
    goalHole->transform.translate = define.goal.pos;
    goalHole->transform.rotation = define.goal.rot;
    AddComponent(goalHole);

    startAnchor = std::make_shared<PlayerStartAnchorComponent>(NextComponentId());
    startAnchor->transform.translate = define.startPos;
    AddComponent(startAnchor);

    // vectorで扱っているが、とりあえず一つだけ
    if (!define.playableAreas.empty())
    {
        const auto& [start, end] = define.playableAreas[0];
        area_start_pos_ = start;
        area_end_pos_ = end;
    }
    else
    {
        area_start_pos_ = {};
        area_end_pos_ = {};
    }
}

void EditorWorld::Save()
{
    const auto& define = GetDefine();
    const auto& filename = std::format("assets/data/stage/{}.json", stage_id_);
    std::ofstream file(filename);
    if (!file.is_open())
    {
        spdlog::error("Failed to open {}", filename);
        return;
    }

    file << define.ToJson().dump(2);

    StageFileManager::UnloadDefine(stage_id_);
    spdlog::info("Stage save successfully {}", filename);
}

void EditorWorld::UpdateComponentIndicator()
{
    const auto& worldPos = GetMouseWorldPos();
    auto sortedComponents = GetComponents();
    std::ranges::sort(sortedComponents.begin(), sortedComponents.end(), [](auto a, auto b)
    {
        return a->zIndex > b->zIndex;
    });
    for (const auto& component : sortedComponents)
    {
        const auto colliderComponent = dynamic_cast<ColliderComponent*>(component);
        if (!colliderComponent)
        {
            continue;
        }
        if (!colliderComponent->collider->Contains(colliderComponent->transform.translate, worldPos))
        {
            continue;
        }

        hovering_component_id_ = colliderComponent->GetId();
        return;
    }

    hovering_component_id_ = -1;
}

void EditorWorld::DrawPreview(DrawStack& stack)
{
    if (stage_id_.empty())
    {
        return;
    }
    if (Game::Device().LeftClicking())
    {
        return;
    }
    if (movement_type_ != MovementType::None)
    {
        return;
    }
    if (hovering_component_id_ != -1 && place_type_ != PlaceType::GoalHole)
    {
        return;
    }

    const auto& worldPos = GetMouseWorldPos();

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * 0.5));

    switch (place_type_)
    {
    case PlaceType::PlayerStart:
        {
            auto component = PlayerStartAnchorComponent(-1);
            component.transform.translate = worldPos;
            component.Draw(&stack);
            break;
        }
    case PlaceType::GoalHole:
        {
            auto component = GoalHoleComponent(-1);
            component.transform.translate = worldPos;
            component.Draw(&stack);
            break;
        }
    case PlaceType::Component:
        {
            auto component = CommonPlanetComponent(-1, planet_radius_, planet_graph_id_);
            component.transform.translate = worldPos;
            component.Draw(&stack);
            break;
        }
    case PlaceType::PlayableArea:
        break;
    }

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void EditorWorld::DrawPlayableArea() const
{
    if (area_start_pos_.Distance(area_end_pos_) < 0.1f)
    {
        return;
    }
    if (place_type_ != PlaceType::PlayableArea)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * 0.20f));
    }
    auto stack = CreateDrawStack();

    stack.Push();
    stack.Translate(area_start_pos_);
    const auto& startPos = stack.GetScreenPos();
    stack.Pop();

    stack.Push();
    stack.Translate(area_end_pos_);
    const auto& endPos = stack.GetScreenPos();
    stack.Pop();

    DrawBoxAA(startPos.x, startPos.y, endPos.x, endPos.y, GetColor(0, 255, 0), false, 2);

    if (place_type_ != PlaceType::PlayableArea)
    {
        const auto& center = Math::GetCenter(startPos, endPos);
        DrawCircleAA(center.x, center.y, 2, 8, GetColor(0, 255, 0), true);
    }

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}

void EditorWorld::DrawGizmo(DrawStack& stack)
{
    const auto& component = GetComponent(selected_component_id_);
    if (!component)
    {
        GetCamera().draggable = true;
        movement_type_ = MovementType::None;
        return;
    }

    stack.Push();
    component->transform.ApplyDrawStack(stack);

    const auto& pos = stack.GetScreenPos();

    // 真ん中丸
    const float radius = movement_type_ == MovementType::Both ? 8 : 4;
    DrawCircleAA(pos.x, pos.y, radius, 8, GetColor(255, 255, 255), true);

    static constexpr float arrow_offset = 16;
    static constexpr float arrow_length = arrow_offset + 32;
    static constexpr float arrow_head = 10;
    static constexpr float arrow_head_length = 16;

    // X軸の矢印
    static const auto x_color = GetColor(255, 0, 0);
    const auto xOpacity = movement_type_ == MovementType::X || movement_type_ == MovementType::None ? 1.0f : 0.5f;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * xOpacity));
    DrawLineAA(pos.x + arrow_offset, pos.y, pos.x + arrow_length, pos.y, x_color, 4);
    DrawTriangleAA(pos.x + arrow_length, pos.y - arrow_head, pos.x + arrow_length, pos.y + arrow_head,
                   pos.x + arrow_length + arrow_head_length, pos.y, x_color, true);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // Y軸の矢印
    static const auto y_color = GetColor(0, 255, 0);
    const auto yOpacity = movement_type_ == MovementType::Y || movement_type_ == MovementType::None ? 1.0f : 0.5f;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * yOpacity));
    DrawLineAA(pos.x, pos.y + arrow_offset, pos.x, pos.y + arrow_length, y_color, 4);
    DrawTriangleAA(pos.x - arrow_head, pos.y + arrow_length, pos.x + arrow_head, pos.y + arrow_length, pos.x,
                   pos.y + arrow_length + arrow_head_length, y_color, true);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    stack.Pop();
}

void EditorWorld::DrawMousePos() const
{
    const auto& pos = GetMouseWorldPos();
    DrawFormatString(8, 8, GetColor(255, 255, 255), "(%d, %d)", static_cast<int>(pos.x), static_cast<int>(pos.y));
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
        static const auto grid_color = GetColor(38, 53, 71);
        DrawLine(-2000, pos.y + gridSizeX, 2000, pos.y + gridSizeX, grid_color);
        DrawLine(pos.x + gridSizeX, -2000, pos.x + gridSizeX, 2000, grid_color);
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

void EditorWorld::PushStyles()
{
    // ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
    // ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
}

void EditorWorld::PopStyles()
{
    // ImGui::PopStyleColor();
    // ImGui::PopStyleColor();
}

StageDefine EditorWorld::GetDefine()
{
    StageDefine define;
    define.startPos = startAnchor->transform.translate;
    define.goal = GoalDefine{
        .pos = goalHole->transform.translate,
        .rot = goalHole->transform.rotation
    };
    for (const int& id : defined_component_ids_)
    {
        if (id == startAnchor->GetId() || id == goalHole->GetId())
        {
            continue;
        }

        const auto& component = GetComponent(id);
        if (!component)
        {
            continue;
        }

        if (const auto& planet = dynamic_cast<CommonPlanetComponent*>(component))
        {
            PlanetDefine planetDefine = {
                .pos = planet->transform.translate,
                .radius = planet->radius,
                .graphId = planet->graphId,
                .gravityMultiplier = planet->gravityMultiplier,
                .isSatellite = planet->isSatellite,
                .rotationSpeed = planet->rotationSpeed,
                .rotationOriginOffsetX = planet->rotationOriginOffset.x,
                .rotationOriginOffsetY = planet->rotationOriginOffset.y
            };
            define.planets.emplace_back(planetDefine);
        }
    }

    define.playableAreas.emplace_back(PlayableAreaDefine{
        .start = area_start_pos_,
        .end = area_end_pos_,
    });

    return define;
}

bool EditorWorld::CanCreateStage(const std::string& id)
{
    if (id.empty())
    {
        return false;
    }
    const auto& ids = StageManager::GetStageIds();
    return std::ranges::find(ids, id) == ids.end();
}

void EditorWorld::CreateStage(const std::string& id)
{
    const auto& filename = std::format("assets/data/stage/{}.json", id);
    std::ofstream file(filename);

    file << "{}";

    file.close();

    Load(id);
}
