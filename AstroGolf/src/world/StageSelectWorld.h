#pragma once
#include "World.h"
#include "../component/select/StagePreviewComponent.h"

class StageSelectComponent;

class StageSelectWorld final : public World
{
    std::shared_ptr<StagePreviewComponent> preview_component_;
    std::shared_ptr<StageSelectComponent> select_component_;
    int background_graph_handle_;

public:
    StageSelectWorld();
    ~StageSelectWorld() override;
    void Draw() override;
    void Update(const float& deltaTime) override;
    [[nodiscard]] WorldType GetType() const override;

protected:
    void DrawBackground(DrawStack& stack) const override;
};
