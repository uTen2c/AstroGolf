#pragma once
#include "World.h"
#include "../component/select/StagePreviewComponent.h"

class StageSelectComponent;

class StageSelectWorld final : public World
{
    std::shared_ptr<StagePreviewComponent> preview_component_;
    std::shared_ptr<StageSelectComponent> select_component_;
    int background_graph_handle_;
    std::vector<std::shared_ptr<Graph>> preview_graphs_ = {};

public:
    StageSelectWorld();
    ~StageSelectWorld() override;
    void Draw() override;
    void DrawUi() override;
    void Update(const float& deltaTime) override;
    [[nodiscard]] WorldType GetType() const override;

protected:
    void DrawBackground(DrawStack& stack) const override;
};
