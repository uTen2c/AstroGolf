#pragma once
#include <map>
#include <string>
#include <vector>

#include "StageManager.h"

struct StageProgressData
{
    bool cleared;
    std::vector<GoalChallengeType> clearedChallenges;
};

class SaveManager
{
    inline static std::map<std::string, StageProgressData> progresses_ = {};

public:
    static void Load();
    static void Save();

    [[nodiscard]] static std::map<std::string, StageProgressData>& GetProgresses();

    /**
     * @return 新たにクリアしたらtrue
     */
    static bool ClearStage(const std::string& stageId);

    /**
     * @return 新たにクリアしたらtrue
     */
    static bool ClearChallenge(const std::string& stageId, GoalChallengeType type);
};
