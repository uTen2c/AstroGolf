#pragma once
#include <string>
#include <unordered_map>

#include "StageDefine.h"

class StageFileManager
{
    inline static std::unordered_map<std::string, std::shared_ptr<StageDefine>> loaded_defines_;

    StageFileManager() = default;

public:
    static std::shared_ptr<StageDefine> GetOrLoadDefine(const std::string& id);
    static std::shared_ptr<StageDefine> LoadDefine(const std::string& id);
    static void UnloadDefine(const std::string& id);
};
