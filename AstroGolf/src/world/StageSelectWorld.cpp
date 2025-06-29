#include "StageSelectWorld.h"

#include <format>
#include <memory>

#include "../Game.h"
#include "../component/select/StageSelectComponent.h"
#include "../game/SaveManager.h"
#include "../game/StageManager.h"
#include "../graph/Graphs.h"

StageSelectWorld::StageSelectWorld()
{
    title_font_handle_ = CreateFontToHandle("M PLUS 1p Bold", 32, 5, DX_FONTTYPE_ANTIALIASING_8X8);

    for (const auto& stage : StageManager::GetStages())
    {
        const auto ptr = std::make_shared<Graph>(std::format("preview/{}", stage.preview), 600, 300);
        preview_graphs_.emplace_back(ptr);
    }

    menuEnabled = false;
    zoomEnabled = false;
}

StageSelectWorld::~StageSelectWorld()
{
    DeleteFontToHandle(title_font_handle_);
}

void StageSelectWorld::Init()
{
    World::Init();

    preview_component_ = std::make_shared<StagePreviewComponent>(NextComponentId());
    select_component_ = std::make_shared<StageSelectComponent>(NextComponentId());

    AddComponent(preview_component_);
    AddComponent(select_component_);

    GetPlayer()->transform.translate = {10000, 10000};
}

void StageSelectWorld::Draw()
{
    World::Draw();

    static constexpr auto title = "ステージ選択";
    const auto width = static_cast<float>(GetDrawStringWidthToHandle(title, std::strlen(title), title_font_handle_));
    DrawStringFToHandle(WINDOW_WIDTH * 0.5f - width * 0.5f, 32.0f, title, GetColor(255, 255, 255),
                        title_font_handle_);
}

void StageSelectWorld::Update(const float& deltaTime)
{
    World::Update(deltaTime);

    auto& camera = GetCamera();
    camera.transform.translate = {};

    if (
        const int& focusedIndex = select_component_->GetFocusedIndex();
        focusedIndex != -1
    )
    {
        if (const auto& graph = preview_graphs_[focusedIndex])
        {
            preview_component_->SetGraph(graph);
        }

        const auto& stage = StageManager::GetStages()[focusedIndex];
        const auto& progress = SaveManager::GetProgresses()[stage.id];
        std::vector<ChallengeEntry> entries;
        std::ranges::transform(
            stage.challenges,
            std::back_inserter(entries),
            [progress](const GoalChallengeType type) -> ChallengeEntry
            {
                return {
                    .cleared = std::ranges::find(progress.clearedChallenges, type) != progress.clearedChallenges.end(),
                    .type = type
                };
            });
        preview_component_->SetChallenges(entries);
    }
}

WorldType StageSelectWorld::GetType() const
{
    return WorldType::StageSelect;
}

void StageSelectWorld::DrawBackground(DrawStack& stack)
{
    Graphs::stageBackground->Draw(0, 0);
}
