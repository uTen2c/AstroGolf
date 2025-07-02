#pragma once
#include "StageWorld.h"
#include "../editor/StageDefine.h"

class PlayWorld final : public StageWorld
{
    std::string id_;
    bool debug_ = false;
    bool free_camera_ = false;

public:
    StageDefine stageDefine;

    explicit PlayWorld(std::string id);
    explicit PlayWorld(std::string id, const StageDefine& stageDefine, bool debug = false);
    void Update(const float& deltaTime) override;
    void Init() override;
    void Reload() const override;

protected:
    void DrawBackground(DrawStack& stack) override;

public:
    void UpdateCamera(float deltaTime);
    
    [[nodiscard]] WorldType GetType() const override;
    [[nodiscard]] std::string GetStageId() const override;
    void OnGoal() override;
};
