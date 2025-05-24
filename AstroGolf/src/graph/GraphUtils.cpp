#include "GraphUtils.h"

#include <DxLib.h>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

int GraphUtils::Load(const std::string& graphName)
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
