#pragma once
#include "StageWorld.h"
#include "World.h"

class Graph;

class TitleWorld final : public StageWorld
{
    float opening_seconds_ = 0;
    bool should_show_cursor_hint_ = false;
    float cursor_animation_seconds_ = 0;
    float cursor_animation_delta_ = 0;
    float cursor_fade_in_delta_ = 0;
    int cursor_hint_font_handle_;

public:
    TitleWorld();
    ~TitleWorld() override;
    void Init() override;
    void Update(const float& deltaTime) override;

    void OnCameraMoveWithMouse(CameraComponent* camera) override;
    void OnGoal() override;

    std::string GetStageId() const override;

protected:
    void DrawBackground(DrawStack& stack) override;
    void PostDraw(DrawStack& stack) override;

private:
    void UpdateCamera(const float& deltaTime) const;
    void ResetCamera() const;

    static Vec2 ClampCameraPos(const Vec2& pos);

    float GetOpeningDelta() const;

public:
    [[nodiscard]] WorldType GetType() const override;
};
