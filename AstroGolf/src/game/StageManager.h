#pragma once

#include <string>
#include <vector>

struct Stage
{
    std::string id;
    std::string name;
    std::string preview;
};

class StageManager
{
    inline static std::vector<Stage> stages_ = {};
    
    StageManager() = default;

public:
    static bool LoadStages();
    static const std::vector<Stage>& GetStages() { return stages_; }
};
