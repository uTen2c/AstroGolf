#pragma once
#include "World.h"

class EditorWorld final : public World
{
    Vec2 clicked_pos_;
    std::string stage_id_;
    std::vector<int> definedComponentIds;

public:
    EditorWorld();
    ~EditorWorld() override;
    void Init() override;
    void Draw() override;

protected:
    void DrawBackground(DrawStack& stack) override;

public:
    void Update(const float& deltaTime) override;
    void UpdatePlanetEditor(const float& deltaTime);
    [[nodiscard]] WorldType GetType() const override;

private:
    void Load(const std::string& stageId);
    void DrawPreview(DrawStack& stack);

    Vec2 GetMouseWorldPos() const;
    
    static void DrawGrid(DrawStack& stack);
    static Vec2 GetMousePos();
};
