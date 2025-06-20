#include "StageSelectWorld.h"

#include <format>
#include <memory>

#include "../component/select/StageSelectComponent.h"
#include "../game/StageManager.h"

StageSelectWorld::StageSelectWorld()
{
    background_graph_ = std::make_unique<Graph>("background.png", 2048, 2048);

    for (const auto& stage : StageManager::GetStages())
    {
        const auto ptr = std::make_shared<Graph>(std::format("preview/{}", stage.preview), 1024, 1024);
        preview_graphs_.emplace_back(ptr);
    }

    menuEnabled = false;
    zoomEnabled = false;
}

StageSelectWorld::~StageSelectWorld() = default;

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
    }
}

WorldType StageSelectWorld::GetType() const
{
    return WorldType::StageSelect;
}

void StageSelectWorld::DrawBackground(DrawStack& stack) const
{
    background_graph_->Draw(0, 0);
}
