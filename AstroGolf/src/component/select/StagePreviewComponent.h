#pragma once
#include "../Component.h"
#include "../../graph/Graph.h"

class StagePreviewComponent final : public Component
{
    std::unique_ptr<Graph> base_graph_;
    std::shared_ptr<Graph> graph_;
    int blur_screen_handle_ = 0;

public:
    int blurHandle = 0;

    explicit StagePreviewComponent(const int id);

    ~StagePreviewComponent() override;

    void Draw(DrawStack* stack) override;

    void SetGraph(const std::shared_ptr<Graph>& g);
};
