#pragma once
#include "World.h"
#include "../component/select/StagePreviewComponent.h"
#include "../component/select/StageSelectComponent.h"


class StageSelectWorld final : public World
{
    std::shared_ptr<StagePreviewComponent> preview_component_;
    std::shared_ptr<StageSelectComponent> select_component_;
    std::vector<std::shared_ptr<Graph>> preview_graphs_ = {};

    int title_font_handle_;

public:
    StageSelectWorld();
    ~StageSelectWorld() override;
    void Init() override;
    void Draw() override;
    void Update(const float& deltaTime) override;
    [[nodiscard]] WorldType GetType() const override;

protected:
    void DrawBackground(DrawStack& stack) override;
};
