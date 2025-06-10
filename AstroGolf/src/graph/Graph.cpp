#include "Graph.h"

#include <DxLib.h>

#include "GraphUtils.h"

Graph::Graph(const std::string& path, const int width, const int height)
    : handle(GraphUtils::Load(path)),
      width(width),
      height(height)
{
}

Graph::~Graph()
{
    DeleteGraph(handle);
}

void Graph::Draw(const DrawStack& stack) const
{
    const auto& pos = stack.GetScreenPos();
    const auto& scale = stack.GetScreenScale();
    const auto& rot = stack.GetScreenRotate();
    DrawRotaGraph3F(pos.x, pos.y, static_cast<float>(width) * 0.5f, static_cast<float>(height) * 0.5f, scale.x, scale.y,
                    rot, handle, true);
}
