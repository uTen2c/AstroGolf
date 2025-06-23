#include "StagePreviewComponent.h"

#include <DxLib.h>

#include "../../Game.h"
#include "../../graph/Graphs.h"

namespace
{
    constexpr auto PREVIEW_WIDTH = 600;
    constexpr auto PREVIEW_HEIGHT = 300;
    constexpr auto BASE_WIDTH = 620;
    constexpr auto BASE_HEIGHT = 320;
    constexpr auto TITLE_OFFSET = 40;
}

StagePreviewComponent::StagePreviewComponent(const int id): Component(id)
{
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

    constexpr auto padding = (WINDOW_HEIGHT - PREVIEW_WIDTH) / 2;
    constexpr auto x = WINDOW_WIDTH - PREVIEW_WIDTH - padding;
    constexpr auto baseOffset = (BASE_WIDTH - PREVIEW_WIDTH) * 0.5f;
    DrawGraphF(x - baseOffset, padding - baseOffset + TITLE_OFFSET, base_graph_->handle, true);
    graph_->Draw(x, padding + TITLE_OFFSET);

    constexpr auto borderWidth = 3;
    constexpr auto challengeBaseX = x - baseOffset;
    const auto challengeBaseY = padding - baseOffset + TITLE_OFFSET + base_graph_->height + 32;
    DrawBoxAA(
        challengeBaseX, challengeBaseY, challengeBaseX + BASE_WIDTH, challengeBaseY + 240,
        GetColor(66, 86, 213), false, borderWidth);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * 0.2f));
    DrawBoxAA(
        challengeBaseX + borderWidth,
        challengeBaseY + borderWidth,
        challengeBaseX + BASE_WIDTH - borderWidth * 2,
        challengeBaseY + 240 - borderWidth * 2,
        GetColor(0, 17, 206), true);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

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
