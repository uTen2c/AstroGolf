#include "GoalScoreDisplayComponent.h"

#include <DxLib.h>

#include "../../Game.h"

GoalScoreDisplayComponent::GoalScoreDisplayComponent(const int id): Component(id)
{
    font_handle_ = CreateFontToHandle("Outfit", 128, 7, DX_FONTTYPE_ANTIALIASING_8X8);
}

GoalScoreDisplayComponent::~GoalScoreDisplayComponent()
{
    DeleteFontToHandle(font_handle_);
}

void GoalScoreDisplayComponent::Update(float delta)
{
    Component::Update(delta);
}

void GoalScoreDisplayComponent::Draw(DrawStack* stack)
{
    if (scoreType == GoalScoreType::None)
    {
        return;
    }

    const auto& scoreText = GetScoreText(scoreType);
    const auto textWidth = static_cast<float>(GetDrawStringWidthToHandle(
        scoreText.c_str(),
        static_cast<int>(scoreText.length()),
        font_handle_
    ));

    auto offset = 4;
    DrawStringFToHandle(
        (WINDOW_WIDTH - textWidth) * 0.5f + offset,
        100 + offset,
        scoreText.c_str(),
        GetColor(255, 255, 255),
        font_handle_
    );
    DrawStringFToHandle(
        (WINDOW_WIDTH - textWidth) * 0.5f - offset,
        100 - offset,
        scoreText.c_str(),
        GetColor(255, 255, 255),
        font_handle_
    );
    DrawStringFToHandle(
        (WINDOW_WIDTH - textWidth) * 0.5f - offset,
        100 + offset,
        scoreText.c_str(),
        GetColor(255, 255, 255),
        font_handle_
    );
    DrawStringFToHandle(
        (WINDOW_WIDTH - textWidth) * 0.5f + offset,
        100 - offset,
        scoreText.c_str(),
        GetColor(255, 255, 255),
        font_handle_
    );

    DrawStringFToHandle(
        (WINDOW_WIDTH - textWidth) * 0.5f + offset,
        100,
        scoreText.c_str(),
        GetColor(255, 255, 255),
        font_handle_
    );

    DrawStringFToHandle(
        (WINDOW_WIDTH - textWidth) * 0.5f - offset,
        100,
        scoreText.c_str(),
        GetColor(255, 255, 255),
        font_handle_
    );

    DrawStringFToHandle(
        (WINDOW_WIDTH - textWidth) * 0.5f,
        100 - offset,
        scoreText.c_str(),
        GetColor(255, 255, 255),
        font_handle_
    );

    DrawStringFToHandle(
        (WINDOW_WIDTH - textWidth) * 0.5f,
        100 + offset,
        scoreText.c_str(),
        GetColor(255, 255, 255),
        font_handle_
    );

    DrawStringFToHandle(
        (WINDOW_WIDTH - textWidth) * 0.5f,
        100,
        scoreText.c_str(),
        GetColor(66, 86, 213),
        font_handle_
    );
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
