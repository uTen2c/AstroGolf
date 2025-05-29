#pragma once
#include "World.h"

class DemoWorld final : public World
{
    int background_graph_handle_;

public:
    DemoWorld();
    ~DemoWorld() override;
    [[nodiscard]] WorldType GetType() const override;

    void Update(const float& deltaTime) override;

protected:
    void DrawBackground(DrawStack& stack) const override;

private:
    void UpdateCamera(const float& deltaTime) const;
};
