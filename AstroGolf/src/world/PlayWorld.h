#pragma once
#include "StageWorld.h"
#include "../component/GoalHoleComponent.h"
#include "../editor/StageDefine.h"

class PlayWorld final : public StageWorld
{
    std::string id_;
    bool debug_ = false;
    bool free_camera_ = false;
    bool out_of_area_ = false; // プレイエリア外に行ったか

public:
    StageDefine stageDefine;

    explicit PlayWorld(std::string id);
    explicit PlayWorld(std::string id, const StageDefine& stageDefine, bool debug = false);
    void Update(const float& deltaTime) override;
    void CheckPlayableArea();
    void Init() override;
    void Reload() const override;

protected:
    void DrawBackground(DrawStack& stack) override;

public:
    void UpdateCamera(float deltaTime);
    
    [[nodiscard]] WorldType GetType() const override;
    [[nodiscard]] std::string GetStageId() const override;
    void OnCameraMoveWithMouse(CameraComponent* camera) override;
    void OnShot() override;
    void OnGoal() override;
};
