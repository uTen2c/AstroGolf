#include "PlayerComponent.h"

#include <DxLib.h>
#include <imgui.h>
#include <memory>
#include <spdlog/spdlog.h>

#include "GoalHoleComponent.h"
#include "../Game.h"
#include "../graph/Graphs.h"
#include "../math/CircleCollider.h"
#include "../math/Math.h"
#include "../random/Random.h"
#include "../sound/Sound.h"
#include "misc/GroundParticleComponent.h"

namespace
{
    constexpr float max_shot_power = 100.0f;
    constexpr float shot_dead_zone = 20.0f;
    constexpr float shot_power_multiplier = 20.0f;

    bool sounds_initialized = false;
    std::unique_ptr<Sound> shot1_sound;
    std::unique_ptr<Sound> shot2_sound;
    std::unique_ptr<Sound> dropping_in_hole_sound;
    std::unique_ptr<Sound> hit_sound;
}

PlayerComponent::PlayerComponent(const int id)
    : PhysicsComponent(id)
{
    collider = std::make_unique<CircleCollider>(radius);
    zIndex = 1100;
    shot_start_transform_ = transform;
    trail_screen_ = MakeScreen(WINDOW_WIDTH, WINDOW_HEIGHT, true);

    if (!sounds_initialized)
    {
        shot1_sound = std::make_unique<Sound>("shot_1.mp3");
        shot2_sound = std::make_unique<Sound>("shot_2.mp3");
        dropping_in_hole_sound = std::make_unique<Sound>("dropping_in_hole.mp3");
        hit_sound = std::make_unique<Sound>("hit.mp3");
        sounds_initialized = true;
    }
}

PlayerComponent::~PlayerComponent()
{
    DeleteGraph(trail_screen_);
}

void PlayerComponent::Update(const float deltaTime)
{
    if (const auto circleCollider = dynamic_cast<CircleCollider*>(collider.get()))
    {
        circleCollider->radius = radius;
    }

    PhysicsComponent::Update(deltaTime);

    if (velocity.Length() > 0)
    {
        if (velocity.Length() < 0.1)
        {
            velocity = {};
        }
        else
        {
            if (intersecting_)
            {
                // 0.5秒でとまる
                velocity.Sub(velocity.Copy().Mul(deltaTime * 2));
            }
            else
            {
                const auto multiplier = 0.99f * (1 - max(250 - lastGravityPower, 0) / 250);
                velocity.Sub(velocity.Copy().Mul(multiplier * deltaTime));
            }
        }
    }

    UpdateShot();
    UpdateDebugMove();
}

void PlayerComponent::UpdateMovement(const float deltaTime)
{
    const auto lastPos = transform.translate;
    PhysicsComponent::UpdateMovement(deltaTime);

    // 接地判定
    landing_ = intersectingNormal.Length() > 0;

    // 接地法線との角度が135deg以上なら打てない
    static constexpr auto over_rad = -45.0f * Math::deg_to_rad;
    valid_shot_angle_ = intersectingNormal.Length() > 0
        && drag_vector_.Length() > 0
        && intersectingNormal.Dot(drag_vector_.Normalized()) >= over_rad;

    const auto& movedDistance = lastPos.Distance(transform.translate);
    last_move_speed_ = movedDistance;
    last_move_speed_vec_ = transform.translate - lastPos;
    should_trails_ = movedDistance > 0.2;
}

void PlayerComponent::Draw(DrawStack* stack)
{
    stack->Push();
    ApplyDrawStack(stack);

    if (Game::debugEnabled && world->GetType() != WorldType::StageSelect)
    {
        ImGui::Begin("Player", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::SeparatorText("Transform");
        ImGui::Text("Translate");
        ImGui::InputFloat("X", &transform.translate.x);
        ImGui::InputFloat("Y", &transform.translate.y);
        ImGui::Separator();
        ImGui::Text("Shot vec: %0.3f, %0.3f", drag_vector_.x, drag_vector_.y);
        ImGui::Text("Shot pow: %0.3f", drag_vector_.Length());
        ImGui::Text("Shot count: %d", shot_count_);
        ImGui::End();
    }

    DrawTrail(stack);

    const auto& screenPos = stack->GetScreenPos();
    screen_pos_ = screenPos;

    if (const auto circleCollider = dynamic_cast<CircleCollider*>(collider.get()))
    {
        const auto& screenScale = stack->GetScreenScale();
        const auto scaledRadius = circleCollider->radius * screenScale.x;
        const auto scaledPosnum = static_cast<int>(max(16 * screenScale.x, 16));
        // 本体描画
        DrawCircleAA(screenPos.x, screenPos.y, scaledRadius, scaledPosnum, GetColor(255, 255, 255), true);
        // コライダー描画
        // DrawCircleAA(screenPos.x, screenPos.y, 1, 4, GetColor(0, 255, 0), false);
        // DrawCircleAA(screenPos.x, screenPos.y, scaledRadius, scaledPosnum, GetColor(0, 255, 0), false);

        if (drag_vector_.Length() > 0)
        {
            const auto ratio = round((drag_vector_.Length() / max_shot_power) * 100) / 100.0f;
            // 1pxとかになると見栄えが悪いので最低8pxにする
            const auto height = std::max<float>(Graphs::playerPowerIndicator->height * ratio, 8);
            const auto offset = radius * 1.5f;
            stack->Push();
            stack->Translate(drag_vector_.Normalized().Mul(offset));
            const auto& pos = stack->GetScreenPos();
            const auto rot = drag_vector_.Normalized().SignedAngle({0, -1});
            const auto srcX = CanShot() ? 0 : 12;
            DrawRectRotaGraph3F(pos.x, pos.y, srcX, 0, 12, height, static_cast<float>(12) * 0.5f, height, 1, 1, rot,
                                Graphs::playerPowerIndicator->handle, true);
            stack->Pop();
        }

        // デバッグ表示
        if (Game::debugEnabled)
        {
            // Gravity sources
            for (const auto& gravitySource : gravitySources)
            {
                auto vec = screenPos;
                vec.Add(gravitySource.Copy().Mul(0.5f));
                DrawLineAA(screenPos.x, screenPos.y, vec.x, vec.y, GetColor(0, 255, 0), 2);
            }

            // Normal
            {
                auto copied = screenPos;
                auto n = intersectingNormal;
                n.Mul(30.0f);
                copied.Add(n);
                DrawLineAA(screenPos.x, screenPos.y, copied.x, copied.y, GetColor(0, 0, 255), 2);
            }

            // Shot vec
            if (isDragging && drag_vector_.Length() > 0)
            {
                const auto offset = radius * 1.5f + Graphs::playerPowerIndicator->height / 2;
                auto vec = drag_vector_;
                const auto arrowStart = screenPos.Copy().Add(vec.Normalized().Mul(offset));
                auto copied = arrowStart;
                const auto blue = vec.Length() / max_shot_power * 255;
                copied.Add(vec.Copy().Normalize().Mul(offset));
                vec.Mul(2);
                copied.Add(vec);

                const auto color = GetColor(255, 0, static_cast<int>(blue));

                DrawLineAA(arrowStart.x, arrowStart.y, copied.x, copied.y, color, 2);

                const auto leftArrowDir = vec.Copy().Neg().Rotate(30 * Math::deg_to_rad).Normalize();
                const auto leftArrowPos = copied.Copy().Add(leftArrowDir.Copy().Mul(16));
                DrawLineAA(copied.x, copied.y, leftArrowPos.x, leftArrowPos.y, color, 2);

                const auto rightArrowDir = vec.Copy().Neg().Rotate(-30 * Math::deg_to_rad).Normalize();
                const auto rightArrowPos = copied.Copy().Add(rightArrowDir.Copy().Mul(16));
                DrawLineAA(copied.x, copied.y, rightArrowPos.x, rightArrowPos.y, color, 2);
            }
        }
    }

    stack->Pop();
}

void PlayerComponent::Respawn()
{
    transform = shot_start_transform_;
}

Vec2 PlayerComponent::GetDragVector() const
{
    return drag_vector_;
}

bool PlayerComponent::CanShot() const
{
    return landing_ && valid_shot_angle_ && world->CanPlayerShot();
}

int PlayerComponent::GetShotCount() const
{
    return shot_count_;
}

void PlayerComponent::UpdateShot()
{
    int x;
    int y;

    GetMousePoint(&x, &y);

    const auto clicking = Game::Device().LeftClicking();
    if (!isDragging && clicking)
    {
        if (
            const auto distance = screen_pos_.Distance({static_cast<float>(x), static_cast<float>(y)});
            distance > radius + 64
        )
        {
            return;
        }
        isDragging = true;
    }

    if (isDragging && !clicking)
    {
        shot_start_transform_ = transform;
        auto shotVec = drag_vector_;
        shotVec.Mul(shot_power_multiplier);

        // 重力の影響を相殺する
        for (const auto& gravitySource : gravitySources)
        {
            shotVec.Add(gravitySource.Copy().Neg());
        }

        if (CanShot())
        {
            velocity = shotVec;
            PlayShotSound();
            shot_count_++;
            world->OnShot();
        }
        drag_vector_ = {};
        isDragging = false;
    }

    if (!isDragging)
    {
        return;
    }

    x -= 640; // FIXME マジックナンバーをやめる
    y -= 360; // FIXME マジックナンバーをやめる

    const auto cameraTranslate = world->GetCamera().transform.translate;
    const auto zoom = world->GetCamera().zoom;
    const auto dx = static_cast<float>(x) / zoom;
    const auto dy = static_cast<float>(y) / zoom;

    auto vec = transform.translate.Copy().Sub({cameraTranslate.x + dx, cameraTranslate.y + dy});
    if (vec.Length() < shot_dead_zone)
    {
        drag_vector_ = {};
        return;
    }
    const auto power = min((vec.Length() - shot_dead_zone) * 0.5f, max_shot_power);
    drag_vector_ = vec.Normalize().Mul(power);
}

void PlayerComponent::UpdateDebugMove()
{
    int x;
    int y;

    GetMousePoint(&x, &y);

    x -= 640; // FIXME マジックナンバーをやめる
    y -= 360; // FIXME マジックナンバーをやめる

    if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0 && CheckHitKey(KEY_INPUT_LSHIFT) != 0)
    {
        auto translate = world->GetCamera().transform.translate;
        velocity = {0, 0};

        const auto zoom = world->GetCamera().zoom;
        Vec2 delta = {static_cast<float>(x), static_cast<float>(y)};
        delta.Div(zoom);

        transform.translate = translate.Add(delta);
    }
}

void PlayerComponent::DrawTrail(DrawStack* stack)
{
    if (trails_.empty())
    {
        for (int i = 0; i < 20; ++i)
        {
            trails_.emplace_back(transform.translate);
        }
    }

    trails_.pop_front();
    trails_.push_back(transform.translate);

    if (!should_trails_)
    {
        return;
    }

    SetDrawScreen(trail_screen_);
    ClearDrawScreen();
    for (const auto& trailPos : trails_)
    {
        stack->Push();
        stack->Translate(transform.translate.Copy().Neg());
        stack->Translate(trailPos);
        const auto distance = transform.translate.Distance(trailPos);
        const auto& screenPos = stack->GetScreenPos();
        const auto scale = (180 - std::clamp(distance, 0.0f, 180.0f)) / 180.0f;
        DrawCircleAA(screenPos.x, screenPos.y, max(10.0f * scale, 3.0f), 16, GetColor(255, 255, 255));
        stack->Pop();
    }
    SetDrawScreen(DX_SCREEN_BACK);

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * 0.75));
    GraphFilter(trail_screen_, DX_GRAPH_FILTER_TWO_COLOR, 128, GetColor(0, 0, 0), 255, GetColor(255, 255, 255), 255);
    GraphFilter(trail_screen_, DX_GRAPH_FILTER_BRIGHT_CLIP, DX_CMP_LESS, 128, true, GetColor(0, 0, 0), 0);
    GraphFilter(trail_screen_, DX_GRAPH_FILTER_GAUSS, 32, 100);
    DrawExtendGraph(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, trail_screen_, true);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void PlayerComponent::PlayShotSound()
{
    const auto& sound = Random::GetInstance().Bool() ? shot1_sound : shot2_sound;
    sound->Play();
}

void PlayerComponent::OnCollide(PhysicsComponent* other, const IntersectingResult& result)
{
    const auto sp = last_move_speed_vec_ * result.normal;
    if (sp.Length() >= 1.5)
    {
        const auto particle = std::make_shared<GroundParticleComponent>(world->NextComponentId());
        particle->transform.translate = result.point;
        particle->normal = result.normal;
        world->AddComponent(particle);
    }

    const auto durationMs = GetNowCount() - last_hit_sound_played_at_;
    if (sp.Length() >= 1.3 && durationMs > 50)
    {
        last_hit_sound_played_at_ = GetNowCount();
        if (dynamic_cast<GoalHoleComponent*>(other))
        {

            dropping_in_hole_sound->volume = std::clamp((last_move_speed_ - 0.3f) / 0.3f, 0.0f, 1.0f);
            dropping_in_hole_sound->Play();
        }
        else
        {
            // hit_sound->Play();
        }
    }
}
