#include "StageSelectWorld.h"

#include <DxLib.h>
#include <memory>

#include "../component/select/StageSelectComponent.h"
#include "../graph/GraphUtils.h"

StageSelectWorld::StageSelectWorld()
{
    background_graph_handle_ = GraphUtils::Load("background.png");

    preview_component_ = std::make_shared<StagePreviewComponent>(NextComponentId());
    select_component_ = std::make_shared<StageSelectComponent>(NextComponentId());

    preview_component_->SetGraph(std::make_shared<Graph>("preview/placeholder.png", 1024, 1024));

    AddComponent(preview_component_);
    AddComponent(select_component_);

    zoomEnabled = false;
    GetPlayer()->transform.translate = {10000, 10000};
}

StageSelectWorld::~StageSelectWorld()
{
    DeleteGraph(background_graph_handle_);
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
}

WorldType StageSelectWorld::GetType() const
{
    return WorldType::StageSelect;
}

void StageSelectWorld::DrawBackground(DrawStack& stack) const
{
    DrawGraphF(0, 0, background_graph_handle_, true);
}
