#include "StageFileManager.h"

#include <spdlog/spdlog.h>
#include <fstream>
#include <memory>

std::shared_ptr<StageDefine> StageFileManager::GetOrLoadDefine(const std::string& id)
{
    if (loaded_defines_.contains(id))
    {
        return loaded_defines_[id];
    }

    return LoadDefine(id);
}

std::shared_ptr<StageDefine> StageFileManager::LoadDefine(const std::string& id)
{
    const auto& filename = fmt::format("assets/data/stage/{}.json", id);
    std::ifstream file(filename);
    if (!file.is_open())
    {
        spdlog::error("Failed to open {}", filename);
        return nullptr;
    }

    json json;
    file >> json;


    auto ptr = std::make_shared<StageDefine>(json);
    loaded_defines_[id] = ptr;
    return ptr;
}

void StageFileManager::UnloadDefine(const std::string& id)
{
    loaded_defines_.erase(id);
}
