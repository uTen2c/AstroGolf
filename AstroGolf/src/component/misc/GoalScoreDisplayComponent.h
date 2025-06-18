#pragma once
#include <string>

#include "../Component.h"
#include "../../graph/Graph.h"

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
    int message_font_handle_;
    int score_font_handle_;
    int movie_handle_;
    int confitti_screen_;
    float animation_seconds_ = 0;
    int star_count_ = 0;
    GoalScoreType score_type_ = GoalScoreType::None;
    int click_start_button_index_ = -1;

    std::unique_ptr<Graph> star_graph_;
    std::unique_ptr<Graph> buttons_graph_;

public:
    explicit GoalScoreDisplayComponent(int id);
    ~GoalScoreDisplayComponent() override;

    void Update(float delta) override;
    void Draw(DrawStack* stack) override;
    void DrawConfitti() const;
    void DrawStars() const;
    void DrawStar(Vec2 pos, bool cleared, float delta) const;
    void DrawButtons();
    void DrawButton(float x, float y, int buttonIndex, const std::function<void()>& onClick);
    static float GetStarDelta(float delaySeconds, float currentSeconds);
    static void DrawOutlinedText(float x, float y, const std::string& text, unsigned int color, float outlineWidth,
                                 unsigned int outlineColor, int fontHandle);
    static void DrawOutlinedTextCentered(float x, float y, const std::string& text, unsigned int color, float outlineWidth,
                                 unsigned int outlineColor, int fontHandle);

    [[nodiscard]] GoalScoreType GetScoreType() const;
    void SetScoreType(GoalScoreType scoreType);

    [[nodiscard]] int GetStarCount() const;
    void SetStarCount(int starCount);

private:
    static std::string GetScoreText(GoalScoreType type);
};
