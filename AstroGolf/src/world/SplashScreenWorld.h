#pragma once
#include "World.h"

class SplashScreenWorld final : public World
{
    float duration_ = 0;
    bool finished_ = false;

public:
    SplashScreenWorld();
    void Init() override;
    void Draw() override;
    void Update(const float& deltaTime) override;
    [[nodiscard]] WorldType GetType() const override;
};
