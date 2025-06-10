#include "Graph.h"

#include <DxLib.h>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include "GraphUtils.h"

namespace
{
    int load(const std::string& graphName)
    {
        const auto handle = LoadGraph(fmt::format("assets/sprite/{}", graphName).c_str());
        if (handle == -1)
        {
            spdlog::warn("Failed to load image file '{}'", graphName);
            return -1;
        }
        spdlog::info("Successfully loaded image file '{}' (handle: {})", graphName, handle);
        return handle;
    }
}

Graph::Graph(const std::string& path, const int width, const int height)
    : handle(load(path)),
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
