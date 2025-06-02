#include "StagePreviewComponent.h"

#include <DxLib.h>

#include "../../Game.h"

namespace
{
    constexpr auto PREVIEW_SIZE = 600;
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
    DrawRectGraphF(x, padding, 0, 0, PREVIEW_SIZE, PREVIEW_SIZE, graph_->handle, true);
}

void StagePreviewComponent::SetGraph(const std::shared_ptr<Graph>& g)
{
    graph_ = g;
}
