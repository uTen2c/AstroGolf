#include "StagePreviewComponent.h"

#include <DxLib.h>

#include "../../Game.h"

namespace
{
    constexpr auto PREVIEW_SIZE = 600;
    constexpr auto BASE_SIZE = 620;
}

StagePreviewComponent::StagePreviewComponent(const int id): Component(id)
{
    base_graph_ = std::make_unique<Graph>("preview_base.png", 1024, 1024);
}

StagePreviewComponent::~StagePreviewComponent()
{
    if (blur_screen_handle_)
    {
        DeleteGraph(blur_screen_handle_);
    }
}

void StagePreviewComponent::Draw(DrawStack* stack)
{
    if (!graph_)
    {
        return;
    }

    constexpr auto padding = (WINDOW_HEIGHT - PREVIEW_SIZE) / 2;
    constexpr auto x = WINDOW_WIDTH - PREVIEW_SIZE - padding;
    constexpr auto baseOffset = (BASE_SIZE - PREVIEW_SIZE) * 0.5f;
    DrawGraphF(x - baseOffset, padding - baseOffset, base_graph_->handle, true);
    DrawRectGraphF(x, padding, 0, 0, PREVIEW_SIZE, PREVIEW_SIZE, graph_->handle, true);
}

void StagePreviewComponent::SetGraph(const std::shared_ptr<Graph>& g)
{
    graph_ = g;
}
