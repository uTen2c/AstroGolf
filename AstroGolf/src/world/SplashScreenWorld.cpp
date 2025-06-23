#include "SplashScreenWorld.h"

#include <algorithm>

#include "TitleWorld.h"
#include "../Game.h"
#include "../graph/Graphs.h"
#include "../math/Math.h"

namespace
{
    bool graphInitialized = false;
    std::unique_ptr<Graph> logo_graph;
}

SplashScreenWorld::SplashScreenWorld()
{
    if (!graphInitialized)
    {
        logo_graph = std::make_unique<Graph>("logo.png", 256, 256);
        graphInitialized = true;
    }
}

void SplashScreenWorld::Init()
{
    World::Init();
}

void SplashScreenWorld::Draw()
{
    DrawStack stack;
    stack.Translate({WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.5f});

    float bgAlpha = 0;

    if (duration_ < 1)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);
    }
    else if (duration_ < 2)
    {
        const auto delta = (duration_ - 1) / 1.0f;
        const auto curvedDelta = Math::EaseOutQuad(delta);
        stack.Push();
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * curvedDelta));
        stack.Rotate((20 - curvedDelta * 20.0f) * Math::deg_to_rad * -1);
        stack.Scale(Math::Lerp(0.5f, 1.0f, curvedDelta));
    }
    else if (duration_ > 3)
    {
        const auto delta = std::clamp((duration_ - 3) / 1.0f, 0.0f, 1.0f);
        const auto curvedDelta = Math::EaseOutQuad(delta);
        stack.Translate(Vec2(0, -720) * curvedDelta);
        bgAlpha = curvedDelta;
    }
    else
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
    }

    logo_graph->Draw(stack);

    if (bgAlpha > 0)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * bgAlpha));
        Graphs::stageBackground->Draw(0, 0);
    }


    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SplashScreenWorld::Update(const float& deltaTime)
{
    World::Update(deltaTime);

    if (duration_ >= 4 && !finished_)
    {
        // Game::instance->ChangeWorldWithTransition<TitleWorld>(TransitionMode::Slide);
        Game::instance->ChangeWorld<TitleWorld>();
        finished_ = true;
    }

    duration_ += deltaTime;
}

WorldType SplashScreenWorld::GetType() const
{
    return WorldType::Splash;
}
