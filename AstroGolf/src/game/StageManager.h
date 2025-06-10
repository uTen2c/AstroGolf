#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "../world/World.h"

struct Stage
{
    std::string id;
    std::string name;
    std::string preview;
};

class StageManager
{
    inline static std::vector<Stage> stages_ = {};
    inline static std::map<std::string, std::function<std::unique_ptr<World>()>> factories_ = {};

    StageManager() = default;

public:
    static inline const std::string demoId = "demo";
    static inline const std::string titleId = "title";

    static bool LoadStages();
    static const std::vector<Stage>& GetStages() { return stages_; }
    static std::unique_ptr<World> CreateWorld(const std::string& id);
    static void OnRegisterWorlds();

private:
    static void RegisterWorld(const std::string& id, const std::function<std::unique_ptr<World>()>& worldFactory);
};
