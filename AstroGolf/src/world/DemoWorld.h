#pragma once
#include "StageWorld.h"
#include "World.h"

class DemoWorld final : public StageWorld
{
public:
    DemoWorld();
    ~DemoWorld() override;
    [[nodiscard]] WorldType GetType() const override;

    void Init() override;
    void Update(const float& deltaTime) override;

    std::string GetStageId() const override;

protected:
    void DrawBackground(DrawStack& stack) override;

private:
    void UpdateCamera(const float& deltaTime) const;
};
