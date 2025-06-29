#pragma once
#include "StageWorld.h"
#include "../editor/StageDefine.h"

class PlayWorld final : public StageWorld
{
    bool debug_;
    
public:
    StageDefine stageDefine;
    

    explicit PlayWorld(const StageDefine& stageDefine, bool debug = false);

    void Update(const float& deltaTime) override;
    void Init() override;
    [[nodiscard]] WorldType GetType() const override;
    [[nodiscard]] std::string GetStageId() const override;
    void OnGoal() override;
};
