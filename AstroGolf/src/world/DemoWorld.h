#pragma once
#include "StageWorld.h"
#include "World.h"

class DemoWorld final : public StageWorld
{
    std::unique_ptr<Graph> background_graph_;

public:
    DemoWorld();
    ~DemoWorld() override;
    [[nodiscard]] WorldType GetType() const override;

    void Init() override;
    void Update(const float& deltaTime) override;
    
    std::string GetStageId() const override;

protected:
    void DrawBackground(DrawStack& stack) const override;

private:
    void UpdateCamera(const float& deltaTime) const;
};
