#pragma once
#include "../Component.h"
#include "../../game/StageManager.h"
#include "../../graph/Graph.h"

struct ChallengeEntry
{
    bool cleared;
    GoalChallengeType type;
};

class StagePreviewComponent final : public Component
{
    std::unique_ptr<Graph> base_graph_;
    std::shared_ptr<Graph> graph_;
    std::vector<ChallengeEntry> challenge_entries_ = {};
    int blur_screen_handle_ = 0;
    int challenge_font_handle_ = 0;

public:
    int blurHandle = 0;

    explicit StagePreviewComponent(int id);

    ~StagePreviewComponent() override;

    void Draw(DrawStack* stack) override;

    void SetGraph(const std::shared_ptr<Graph>& g);

    void SetChallenges(const std::vector<ChallengeEntry>& entries);

private:
    void DrawChallenge(int index, bool cleared, const std::string& text) const;
};
