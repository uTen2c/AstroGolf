#include "Game.h"

#include <algorithm>
#include <DxLib.h>
#include <imgui.h>
#include <spdlog/spdlog.h>

#include "world/DemoWorld.h"
#include "world/StageSelectWorld.h"
#include "world/TitleWorld.h"


Game::Game() = default;

World& Game::GetWorld() const
{
    return *world_;
}

void Game::Update()
{
    if (CheckHitKey(KEY_INPUT_1) != 0 && world_->GetType() != WorldType::Title)
    {
        ChangeWorldWithTransition<TitleWorld>(TransitionMode::Slide);
    }
    if (CheckHitKey(KEY_INPUT_2) != 0 && world_->GetType() != WorldType::Demo)
    {
        ChangeWorldWithTransition<DemoWorld>(TransitionMode::Slide);
    }
    if (CheckHitKey(KEY_INPUT_3) != 0 && world_->GetType() != WorldType::StageSelect)
    {
        ChangeWorldWithTransition<StageSelectWorld>(TransitionMode::Slide);
    }

    if (!isPaused)
    {
        world_->Update(deltaTime);
    }

    world_->Draw();

    if (!isPaused)
    {
        world_->PostUpdate(deltaTime);
    }

    UpdateTransition(deltaTime);
}

void Game::UpdateTransition(const float delta)
{
    if (!changing_)
    {
        return;
    }

    transition_delta_ += delta;

    if (transition_delta_ > max_transition_delta_)
    {
        transition_delta_ = 0;
        changing_ = false;
        world_changed_ = false;
        return;
    }

    if (
        const auto progress = GetTransitionProgress(transition_delta_, max_transition_delta_);
        progress > 0
    )
    {
        if (last_transition_progress_ > progress && !world_changed_)
        {
            world_ = std::move(transition_world_);
            isPaused = false;
            world_changed_ = true;
        }
        const auto curvedProgress = std::clamp((progress * progress) * 2, 0.f, 1.f);
        if (world_changed_)
        {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(curvedProgress * 255));
            DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT * curvedProgress, GetColor(0, 0, 0), true);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }
        else
        {
            // 下からスライド
            if (transition_mode_ == TransitionMode::Slide)
            {
                SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(curvedProgress * 255));
                DrawBox(0, WINDOW_HEIGHT - WINDOW_HEIGHT * curvedProgress, WINDOW_WIDTH, WINDOW_HEIGHT,
                        GetColor(0, 0, 0),
                        true);
                SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            }

            // プレイヤーから広がっていく丸
            if (transition_mode_ == TransitionMode::Circle)
            {
                auto stack = DrawStack();

                // カメラの位置を補正する
                const auto camera = world_->GetCamera();
                auto originOffset = Vec2(640, 360); // FIXME マジックナンバーをやめる
                originOffset.Div(camera.zoom);

                auto trans = camera.transform.translate;
                trans.Mul(-1);
                trans.Add(originOffset);
                stack.Translate(trans);
                stack.Scale(camera.zoom);

                stack.Translate(world_->GetPlayer()->transform.translate);
                const auto center = stack.GetScreenPos();
                DrawCircleAA(center.x, center.y, WINDOW_WIDTH * progress * 0.75f, 16, 0);
            }
        }
        last_transition_progress_ = progress;
    }
}

float Game::GetTransitionProgress(const float elapsedTimeMs, const float durationMs)
{
    if (durationMs <= 0.0f)
    {
        return 0.0f;
    }

    // 正規化された時間 (0.0 から 2.0 の範囲)
    float normalizedTime = elapsedTimeMs / durationMs * 2.0f;

    // 2.0を超えた場合は2.0で剰余を取る（ループ処理）
    normalizedTime = fmod(normalizedTime, 2.0f);

    // 0.0-1.0の範囲では上昇、1.0-2.0の範囲では下降
    if (normalizedTime <= 1.0f)
    {
        return normalizedTime; // 0.0 → 1.0
    }

    return 2.0f - normalizedTime; // 1.0 → 0.0
}
