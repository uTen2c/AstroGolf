#pragma once
#include "World.h"
#include "../component/GoalHoleComponent.h"
#include "../component/editor/PlayerStartAnchorComponent.h"
#include "../editor/StageDefine.h"

enum class PlaceType : std::uint8_t
{
    PlayerStart,
    GoalHole,
    Component,
    PlayableArea,
};

inline const char* to_string(const PlaceType e)
{
    switch (e)
    {
    case PlaceType::PlayerStart: return "PlayerStart";
    case PlaceType::GoalHole: return "Goal";
    case PlaceType::Component: return "Component";
    case PlaceType::PlayableArea: return "PlayableArea";
    }
    return "unknown";
}

enum class MovementType : std::uint8_t
{
    None,
    X,
    Y,
    Both,
};

inline const char* to_string(const MovementType e)
{
    switch (e)
    {
    case MovementType::None: return "None";
    case MovementType::X: return "X";
    case MovementType::Y: return "Y";
    case MovementType::Both: return "Both";
    }
    return "unknown";
}

class EditorWorld final : public World
{
    bool has_define_ = false;
    StageDefine stage_define_;
    Vec2 clicked_pos_;
    std::string stage_id_;
    std::vector<int> defined_component_ids_;
    PlaceType place_type_ = PlaceType::Component;
    int hovering_component_id_ = -1;
    int selected_component_id_ = -1;
    MovementType movement_type_ = MovementType::None;
    Vec2 movement_start_mouse_pos_;
    Vec2 movement_start_component_pos_;
    std::string planet_graph_id_ = "purple";
    float planet_radius_ = 175;

    // PlayableAreaEditor
    bool is_area_dragging_ = false;
    Vec2 area_start_pos_;
    Vec2 area_end_pos_;

    // New stage popup
    std::string new_stage_id_;

public:
    std::shared_ptr<PlayerStartAnchorComponent> startAnchor;
    std::shared_ptr<GoalHoleComponent> goalHole;

    EditorWorld();
    explicit EditorWorld(const std::string& id, const StageDefine& stageDefine);
    ~EditorWorld() override;
    void Init() override;
    void Draw() override;

protected:
    void DrawBackground(DrawStack& stack) override;
    void PostDraw(DrawStack& stack) override;

public:
    void Update(const float& deltaTime) override;
    [[nodiscard]] WorldType GetType() const override;

private:
    void Load(const std::string& stageId);
    void Save();

    void UpdateComponentIndicator();
    void UpdatePlanetEditor(const float& deltaTime);
    void UpdatePlayableAreaEditor();
    void PlacePlayerStartAnchor(const Vec2& pos);
    void PlaceGoalHole(const Vec2& pos);
    void UpdateMovement();
    void UpdateUi();
    void UpdateDelete();

    void DrawGeneralEditor();
    void DrawPreview(DrawStack& stack);
    void DrawPlayableArea() const;
    void DrawGizmo(DrawStack& stack);
    void DrawMousePos() const;

    [[nodiscard]] Vec2 GetMouseWorldPos() const;

    StageDefine GetDefine();

    static bool CanCreateStage(const std::string& id);
    void CreateStage(const std::string& id);
    
    static void DrawGrid(DrawStack& stack);
    static Vec2 GetMousePos();
    static void PushStyles();
    static void PopStyles();
};
