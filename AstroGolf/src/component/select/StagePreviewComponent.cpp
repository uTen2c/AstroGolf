#include "StagePreviewComponent.h"

#include <DxLib.h>

#include "../../Game.h"
#include "../../graph/Graphs.h"

namespace
{
    constexpr float PREVIEW_WIDTH = 640;
    constexpr float PREVIEW_HEIGHT = 360;
    constexpr float PADDING = 32;
    constexpr float INNER_PADDING = 16;
    constexpr float CHALLENGES_PADDING = 8;
    constexpr float BASE_WIDTH = PREVIEW_WIDTH + INNER_PADDING * 2;
    constexpr float BASE_HEIGHT = WINDOW_HEIGHT - INNER_PADDING * 2;
    constexpr float TITLE_OFFSET = 40;

    bool graph_initialized = false;
    std::unique_ptr<Graph> challenges_graph;
}

StagePreviewComponent::StagePreviewComponent(const int id): Component(id)
{
    if (!graph_initialized)
    {
        challenges_graph = std::make_unique<Graph>("stage_select/challenges.png", 398, 44);
    }
    base_graph_ = std::make_unique<Graph>("preview_base.png", 620, 320);
    challenge_font_handle_ = CreateFontToHandle("M PLUS 1p Medium", 24, 5, DX_FONTTYPE_ANTIALIASING_8X8);
}

StagePreviewComponent::~StagePreviewComponent()
{
    if (blur_screen_handle_)
    {
        DeleteGraph(blur_screen_handle_);
    }
    if (challenge_font_handle_)
    {
        DeleteFontToHandle(challenge_font_handle_);
    }
}

void StagePreviewComponent::Draw(DrawStack* stack)
{
    if (!graph_)
    {
        return;
    }

    // 右の枠
    static constexpr float start_x = WINDOW_WIDTH - BASE_WIDTH - PADDING;
    static constexpr float start_y = PADDING;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * 0.5f));
    DrawBoxAA(start_x, start_y, WINDOW_WIDTH - PADDING, WINDOW_HEIGHT - PADDING, GetColor(158, 228, 255), true);

    // 下のChallengesの装飾文字
    static const float challenges_x = WINDOW_WIDTH - PADDING - CHALLENGES_PADDING - challenges_graph->width;
    static const float challenges_y = WINDOW_HEIGHT - PADDING - CHALLENGES_PADDING - challenges_graph->height;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * 0.25f));
    challenges_graph->Draw(challenges_x, challenges_y);

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    static constexpr float preview_x = start_x + INNER_PADDING;
    static constexpr float preview_y = start_y + INNER_PADDING;
    graph_->Draw(preview_x, preview_y);

    for (int i = 0; i < min(challenge_entries_.size(), 3); ++i)
    {
        const auto& [cleared, type] = challenge_entries_[i];
        DrawChallenge(i, cleared, StageManager::GetChallengeMessage(type));
    }
}

void StagePreviewComponent::SetGraph(const std::shared_ptr<Graph>& g)
{
    graph_ = g;
}

void StagePreviewComponent::SetChallenges(const std::vector<ChallengeEntry>& entries)
{
    challenge_entries_ = entries;
}

void StagePreviewComponent::DrawChallenge(int index, bool cleared, const std::string& text) const
{
    constexpr auto padding = (WINDOW_HEIGHT - PREVIEW_WIDTH) / 2;
    constexpr auto baseOffset = (BASE_WIDTH - PREVIEW_WIDTH) * 0.5f;
    constexpr auto baseX = WINDOW_WIDTH - PREVIEW_WIDTH - padding;
    constexpr auto challengeBaseX = baseX - baseOffset;
    const auto challengeBaseY = padding - baseOffset + TITLE_OFFSET + base_graph_->height + 32;
    constexpr auto challengeBaseHeight = 240;
    constexpr auto height = (challengeBaseHeight - 64) / 3.0f;
    const auto x = challengeBaseX + 32;
    const auto y = challengeBaseY + height * index + 32;
    Graphs::starsGraph->Draw(x, y + (height - 32) / 2.0f, cleared ? 1 : 0);
    DrawStringFToHandle(x + Graphs::starsGraph->width + 16, y + (height - 32) / 2.0f + (32 - 24) / 2.0f, text.c_str(),
                        GetColor(255, 255, 255),
                        challenge_font_handle_);
}
