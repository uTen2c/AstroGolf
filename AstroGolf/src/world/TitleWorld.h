#pragma once
#include "StageWorld.h"
#include "World.h"

class Graph;

class TitleWorld final : public StageWorld
{
    std::unique_ptr<Graph> title_graph_;
    float opening_seconds_ = 0;

public:
    TitleWorld();
    ~TitleWorld() override;
    void Init() override;
    void Update(const float& deltaTime) override;

    void OnCameraMoveWithMouse(CameraComponent* camera) override;
    void OnGoal() override;

    std::string GetStageId() const override;

    static void LoadResources();

protected:
    void DrawBackground(DrawStack& stack) const override;

private:
    void UpdateCamera(const float& deltaTime) const;
    void ResetCamera() const;

    static Vec2 ClampCameraPos(const Vec2& pos);

    float GetOpeningDelta() const;

public:
    [[nodiscard]] WorldType GetType() const override;
};
