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

void Graph::DrawCenter(const float& x, const float& y) const
{
    DrawGraphF(x - static_cast<float>(width) * 0.5f, y - static_cast<float>(height) * 0.5f, handle, true);
}

void Graph::Draw(const DrawStack& stack) const
{
    const auto& pos = stack.GetScreenPos();
    const auto& scale = stack.GetScreenScale();
    DrawRotaGraph3F(pos.x, pos.y, 0, 0, scale.x, scale.y, 0, handle, true);
}
