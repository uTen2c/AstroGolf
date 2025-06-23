#include "GoalScoreDisplayComponent.h"

#include <algorithm>
#include <DxLib.h>
#include <imgui.h>
#include <windows.h>
#include <spdlog/spdlog.h>

#include "../../Game.h"
#include "../../math/BoundingBox.h"
#include "../../math/Math.h"
#include "../../world/StageWorld.h"
#include "../../world/StageSelectWorld.h"

GoalScoreDisplayComponent::GoalScoreDisplayComponent(const int id): Component(id)
{
    confitti_screen_ = MakeScreen(1280, 720, true);
    message_font_handle_ = CreateFontToHandle("M PLUS 1p Medium", 32, 5, DX_FONTTYPE_ANTIALIASING_8X8);
    score_font_handle_ = CreateFontToHandle("Outfit", 128, 7, DX_FONTTYPE_ANTIALIASING_8X8);
    movie_handle_ = LoadGraph("assets/movie/confetti.mp4");
    star_graph_ = std::make_unique<Graph>("stage/challenge_star.png", 72, 72);
    buttons_graph_ = std::make_unique<Graph>("stage/result_buttons.png", 400, 70);

    zIndex = 2000;
}

GoalScoreDisplayComponent::~GoalScoreDisplayComponent()
{
    DeleteFontToHandle(message_font_handle_);
    DeleteFontToHandle(score_font_handle_);
    DeleteGraph(movie_handle_);
    DeleteGraph(confitti_screen_);
}

void GoalScoreDisplayComponent::Update(const float delta)
{
    if (Game::debugEnabled)
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
    }

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
    DrawButtons();

    DrawOutlinedTextCentered(
        WINDOW_WIDTH * 0.5f, 150,
        GetScoreText(score_type_), GetColor(66, 86, 213),
        4, GetColor(255, 255, 255),
        score_font_handle_
    );
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

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GoalScoreDisplayComponent::DrawOutlinedText(
    const float x,
    const float y,
    const std::string& text,
    const unsigned int color,
    const float outlineWidth,
    const unsigned int outlineColor,
    const int fontHandle
)
{
    for (float offsetXMul = -1; offsetXMul <= 1; offsetXMul += 0.5f)
    {
        for (float offsetYMul = -1; offsetYMul <= 1; offsetYMul += 0.5f)
        {
            const auto offset = Vec2(offsetXMul, offsetYMul).Normalize().Mul(outlineWidth);
            DrawStringFToHandle(
                x + offset.x,
                y + offset.y,
                text.c_str(),
                outlineColor,
                fontHandle
            );
        }
    }

    DrawStringFToHandle(
        x,
        y,
        text.c_str(),
        color,
        fontHandle
    );
}

void GoalScoreDisplayComponent::DrawOutlinedTextCentered(
    const float x,
    const float y,
    const std::string& text,
    const unsigned int color,
    const float outlineWidth, const unsigned int outlineColor,
    const int fontHandle)
{
    const auto textWidth = static_cast<float>(GetDrawStringWidthToHandle(
        text.c_str(),
        static_cast<int>(text.length()),
        fontHandle
    ));
    DrawOutlinedText(x - textWidth * 0.5f, y, text, color, outlineWidth, outlineColor, fontHandle);
}

void GoalScoreDisplayComponent::DrawStars() const
{
    static constexpr float delay = 0.6f;
    auto currentDelay = 1.0f;
    const auto star1Delta = GetStarDelta(currentDelay, animation_seconds_);
    currentDelay += delay;
    const auto star2Delta = GetStarDelta(currentDelay, animation_seconds_);
    currentDelay += delay;
    const auto star3Delta = GetStarDelta(currentDelay, animation_seconds_);

    constexpr auto centerX = static_cast<float>(WINDOW_WIDTH) * 0.5f;
    constexpr auto y = 360;
    constexpr auto offset = 84;

    DrawStar({centerX - offset, y}, true, star1Delta);
    DrawStar({centerX + offset, y}, true, star2Delta);
    DrawStar({centerX, y - 8}, false, star3Delta);
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
    stack.Push();
    stack.Scale(Math::Lerp(1.75f, 1.0f, delta));
    star_graph_->Draw(stack, cleared ? 0 : 1, 0);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GoalScoreDisplayComponent::DrawButtons()
{
    static constexpr float button_y = WINDOW_HEIGHT - 220;
    static constexpr float gap = 12;

    // ステージセレクトボタン
    DrawButton(WINDOW_WIDTH / 2.0f, button_y, 0, []
    {
        Game::instance->ChangeWorldWithTransition<StageSelectWorld>(TransitionMode::Circle);
    });

    // リトライボタン
    DrawButton(WINDOW_WIDTH / 2.0f, button_y + static_cast<float>(buttons_graph_->height) + gap, 1, [this]
    {
        if (const auto stage = dynamic_cast<StageWorld*>(world))
        {
            stage->Reload();
        }
    });

    if (Game::Device().LeftReleased())
    {
        click_start_button_index_ = -1;
    }
}

void GoalScoreDisplayComponent::DrawButton(const float x, const float y, const int buttonIndex,
                                           const std::function<void()>& onClick)
{
    int mouseX;
    int mouseY;
    GetMousePoint(&mouseX, &mouseY);

    const auto& bb = BoundingBox(buttons_graph_->width, buttons_graph_->height);
    const bool& contains = bb.Contains(Vec2(x, y), Vec2(mouseX, mouseY));
    const bool& hovering = contains && !Game::instance->isPaused;

    const float yOffset = hovering ? -3 : 0;
    const int tileYOffset = hovering ? 1 : 0;
    buttons_graph_->DrawCenter(x, y + yOffset, 0, buttonIndex * 2 + tileYOffset);

    if (Game::Device().LeftClicked() && hovering && click_start_button_index_ == -1)
    {
        click_start_button_index_ = buttonIndex;
    }

    if (Game::Device().LeftReleased() && hovering && click_start_button_index_ == buttonIndex)
    {
        click_start_button_index_ = -1;
        if (onClick)
        {
            onClick();
        }
    }
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
