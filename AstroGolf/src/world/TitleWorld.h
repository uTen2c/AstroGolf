#pragma once
#include "World.h"

class Graph;

class TitleWorld final : public World
{
    int background_graph_handle_;
    std::unique_ptr<Graph> title_graph_;

public:
    TitleWorld();
    ~TitleWorld() override;
    void Update(const float& deltaTime) override;

    void OnCameraMoveWithMouse(CameraComponent* camera) override;

protected:
    void DrawBackground(DrawStack& stack) const override;

private:
    void UpdateCamera(const float& deltaTime);

    static Vec2 ClampCameraPos(const Vec2& pos);

public:
    [[nodiscard]] WorldType GetType() const override;
};
