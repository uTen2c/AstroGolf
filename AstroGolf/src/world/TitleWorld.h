#pragma once
#include "StageWorld.h"
#include "World.h"

class Graph;

class TitleWorld final : public StageWorld
{
    std::unique_ptr<Graph> background_graph_;
    std::unique_ptr<Graph> title_graph_;

public:
    TitleWorld();
    ~TitleWorld() override;
    void Update(const float& deltaTime) override;

    void OnCameraMoveWithMouse(CameraComponent* camera) override;
    void OnGoal() override;

    std::string GetStageId() const override;

protected:
    void DrawBackground(DrawStack& stack) const override;

private:
    void UpdateCamera(const float& deltaTime) const;
    void ResetCamera() const;

    static Vec2 ClampCameraPos(const Vec2& pos);

public:
    [[nodiscard]] WorldType GetType() const override;
};
