#include "GoalScoreDisplayComponent.h"

#include <algorithm>
#include <DxLib.h>
#include <imgui.h>

#include "../../Game.h"
#include "../../math/Math.h"

GoalScoreDisplayComponent::GoalScoreDisplayComponent(const int id): Component(id)
{
    confitti_screen_ = MakeScreen(1280, 720, true);
    font_handle_ = CreateFontToHandle("Outfit", 128, 7, DX_FONTTYPE_ANTIALIASING_8X8);
    movie_handle_ = LoadGraph("assets/movie/confetti.mp4");
    star_graph_ = std::make_unique<Graph>("stage/challenge_star.png", 96, 96);

    zIndex = 2000;
}

GoalScoreDisplayComponent::~GoalScoreDisplayComponent()
{
    DeleteFontToHandle(font_handle_);
    DeleteGraph(movie_handle_);
    DeleteGraph(confitti_screen_);
}

void GoalScoreDisplayComponent::Update(const float delta)
{
    ImGui::Begin("Type");

    if (ImGui::Button("Hole-in-One"))
    {
        SetScoreType(GoalScoreType::HoleInOne);
    }

    if (ImGui::Button("Reset"))
    {
        SetScoreType(GoalScoreType::None);
    }

    ImGui::End();

    Component::Update(delta);
    animation_seconds_ += delta;
}

void GoalScoreDisplayComponent::Draw(DrawStack* stack)
{
    if (score_type_ == GoalScoreType::None)
    {
        return;
    }

    DrawConfitti();
    DrawStars();

    static constexpr auto text_y = 150.0f;

    const auto& scoreText = GetScoreText(score_type_);
    const auto textWidth = static_cast<float>(GetDrawStringWidthToHandle(
        scoreText.c_str(),
        static_cast<int>(scoreText.length()),
        font_handle_
    ));

    for (float offsetXMul = -1; offsetXMul <= 1; offsetXMul += 0.5f)
    {
        for (float offsetYMul = -1; offsetYMul <= 1; offsetYMul += 0.5f)
        {
            const auto offset = Vec2(offsetXMul, offsetYMul).Normalize().Mul(4);
            DrawStringFToHandle(
                (WINDOW_WIDTH - textWidth) * 0.5f + offset.x,
                text_y + offset.y,
                scoreText.c_str(),
                GetColor(255, 255, 255),
                font_handle_
            );
        }
    }

    DrawStringFToHandle(
        (WINDOW_WIDTH - textWidth) * 0.5f,
        text_y,
        scoreText.c_str(),
        GetColor(66, 86, 213),
        font_handle_
    );
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GoalScoreDisplayComponent::DrawConfitti() const
{
    const auto delta = std::clamp(animation_seconds_ * 4.0f, 0.0f, 1.0f);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * delta));
    SetDrawScreen(confitti_screen_);
    DrawGraph(0, 0, movie_handle_, false);
    GraphFilter(confitti_screen_, DX_GRAPH_FILTER_BRIGHT_CLIP, DX_CMP_LESS, 128, true, GetColor(0, 0, 0), 0);
    SetDrawScreen(DX_SCREEN_BACK);
    DrawExtendGraph(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, confitti_screen_, true);
}

void GoalScoreDisplayComponent::DrawStars() const
{
    static constexpr float delay = 0.6f;
    auto currentDelay = 0.25f;
    const auto star1Delta = GetStarDelta(currentDelay, animation_seconds_);
    currentDelay += delay;
    const auto star2Delta = GetStarDelta(currentDelay, animation_seconds_);
    currentDelay += delay;
    const auto star3Delta = GetStarDelta(currentDelay, animation_seconds_);

    constexpr auto centerX = static_cast<float>(WINDOW_WIDTH) * 0.5f;
    constexpr auto y = 400;
    constexpr auto offset = 100;

    DrawStar({centerX - offset, y}, true, star1Delta);
    DrawStar({centerX + offset, y}, true, star2Delta);
    DrawStar({centerX, y - 20}, false, star3Delta);
}

void GoalScoreDisplayComponent::DrawStar(const Vec2 pos, const bool cleared, const float delta) const
{
    if (!cleared)
    {
        star_graph_->DrawCenter(pos.x, pos.y, 1, 0);
        return;
    }

    if (delta < 1)
    {
        star_graph_->DrawCenter(pos.x, pos.y, 1, 0);
    }

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * delta));
    DrawStack stack;
    stack.Translate(pos);
    stack.Scale(Math::Lerp(1.75f, 1.0f, delta));
    star_graph_->Draw(stack, cleared ? 0 : 1, 0);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

float GoalScoreDisplayComponent::GetStarDelta(const float delaySeconds, const float currentSeconds)
{
    static constexpr float duration_seconds = 0.25f;
    const auto d = max(currentSeconds - delaySeconds, 0);
    return min(d / duration_seconds, 1);
}

GoalScoreType GoalScoreDisplayComponent::GetScoreType() const
{
    return score_type_;
}

void GoalScoreDisplayComponent::SetScoreType(const GoalScoreType scoreType)
{
    if (score_type_ == scoreType)
    {
        return;
    }
    score_type_ = scoreType;
    animation_seconds_ = 0.0f;
    PlayMovieToGraph(movie_handle_, DX_PLAYTYPE_LOOP);
}

int GoalScoreDisplayComponent::GetStarCount() const
{
    return star_count_;
}

void GoalScoreDisplayComponent::SetStarCount(int starCount)
{
    star_count_ = std::clamp(starCount, 0, 3);
}

std::string GoalScoreDisplayComponent::GetScoreText(GoalScoreType type)
{
    switch (type)
    {
    case GoalScoreType::HoleInOne:
        return "HOLE-in-ONE";
    case GoalScoreType::Excellent:
        return "Excellent";
    case GoalScoreType::Awesome:
        return "Awesome";
    case GoalScoreType::Good:
        return "Good";
    case GoalScoreType::None:
        return "Unset";
    }
    return "";
}
