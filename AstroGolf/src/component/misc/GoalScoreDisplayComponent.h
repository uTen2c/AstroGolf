#pragma once
#include <string>

#include "../Component.h"

enum class GoalScoreType : std::uint8_t
{
    HoleInOne,
    Excellent,
    Awesome,
    Good,
    None,
};

class GoalScoreDisplayComponent final : public Component
{
    int font_handle_;
    int outline_font_handle_;

public:
    GoalScoreType scoreType = GoalScoreType::None;
    
    explicit GoalScoreDisplayComponent(int id);
    ~GoalScoreDisplayComponent() override;

    void Update(float delta) override;
    void Draw(DrawStack* stack) override;

private:
    static std::string GetScoreText(GoalScoreType type);
};
