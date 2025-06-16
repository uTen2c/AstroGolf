#include "World.h"

#include <algorithm>
#include <DxLib.h>
#include <imgui.h>
#include <ranges>
#include <spdlog/spdlog.h>

#include "StageSelectWorld.h"
#include "StageWorld.h"
#include "../Game.h"
#include "../game/StageManager.h"
#include "../graph/Graph.h"
#include "../math/Math.h"

World::World()
{
    spdlog::info("World init");

    menu_button_graph_ = std::make_unique<Graph>("menu_button.png", 72, 72);
    menu_background_graph_ = std::make_unique<Graph>("menu/menu_background.png", 1024, 720);
    menu_buttons_graph_ = std::make_unique<Graph>("menu/buttons.png", 512, 70);

    camera_ = std::make_shared<CameraComponent>(NextComponentId());
    player_ = std::make_shared<PlayerComponent>(NextComponentId());
    ballistic_ = std::make_shared<BallisticComponent>(NextComponentId());

    AddComponent(camera_);
    AddComponent(player_);
    AddComponent(ballistic_);
}

void World::Draw()
{
    auto stack = DrawStack();

    // カメラの位置を補正する
    auto originOffset = Vec2(640, 360); // FIXME マジックナンバーをやめる
    originOffset.Div(camera_->zoom);

    auto trans = camera_->transform.translate;
    trans.Mul(-1);
    trans.Add(originOffset);
    stack.Translate(trans);
    stack.Scale(camera_->zoom);

    DrawBackground(stack);

    // X, Yの十字線描画
    // stack.Push();
    // const auto& pos = stack.GetScreenPos();
    // DrawLine(-2000, pos.y, 2000, pos.y, GetColor(255, 0, 0));
    // DrawLine(pos.x, -2000, pos.x, 2000, GetColor(0, 255, 0));
    // stack.Pop();

    auto components = GetComponents();
    std::ranges::sort(components, [](const Component* a, const Component* b)
    {
        return a->zIndex < b->zIndex;
    });
    for (const auto& component : components)
    {
        component->Draw(&stack);
    }

    DrawBallistic();

    DrawUi();
}

void World::Update(const float& deltaTime)
{
    const auto components = GetComponents();
    for (const auto component : components)
    {
        if (dynamic_cast<BallisticComponent*>(component))
        {
            continue;
        }
        component->Update(deltaTime);
    }
    for (const auto component : components)
    {
        if (dynamic_cast<BallisticComponent*>(component))
        {
            continue;
        }
        if (const auto physComp = dynamic_cast<PhysicsComponent*>(component))
        {
            physComp->UpdateMovement(deltaTime);
        }
    }
}

void World::PostUpdate(const float& deltaTime)
{
    for (const auto component : GetComponents())
    {
        if (dynamic_cast<BallisticComponent*>(component))
        {
            continue;
        }
        component->PostUpdate(deltaTime);
    }
}

void World::DrawUi()
{
    int mouseX;
    int mouseY;
    GetMousePoint(&mouseX, &mouseY);

    if (!menu_opened_)
    {
        constexpr auto menuButtonPadding = 24;
        const auto halfW = static_cast<float>(menu_button_graph_->width) * 0.5f;
        const auto halfH = static_cast<float>(menu_button_graph_->height) * 0.5f;
        const auto centerX = WINDOW_WIDTH - halfW - menuButtonPadding;
        const auto centerY = halfH + menuButtonPadding;
        const auto isHovering = Math::InRange(static_cast<float>(mouseX), centerX - halfW, centerX + halfW) &&
            Math::InRange(static_cast<float>(mouseY), centerY - halfH, centerY + halfH);
        menu_button_graph_->DrawCenter(centerX, centerY, isHovering ? 1 : 0, 0);

        if (isHovering && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
        {
            SetMenuOpen(true);
        }
    }

    // Esc押したら閉じる
    if (CheckHitKey(KEY_INPUT_ESCAPE) != 0)
    {
        if (!menu_key_pressing_)
        {
            SetMenuOpen(!menu_opened_);
            menu_key_pressing_ = true;
        }
    }
    else
    {
        menu_key_pressing_ = false;
    }

    if (!menu_opened_)
    {
        return;
    }

    menu_background_graph_->Draw(WINDOW_WIDTH - menu_background_graph_->width, 0);

    constexpr auto padding = 24;
    constexpr auto buttonCount = 3;
    const auto buttonX = WINDOW_WIDTH - menu_buttons_graph_->width - padding;
    const auto offsetY = WINDOW_HEIGHT - menu_buttons_graph_->height - padding;
    for (int i = 0; i <= buttonCount; ++i)
    {
        constexpr auto gap = 24;
        const auto tileY = i * 2;
        const auto buttonY = offsetY + (i - buttonCount) * (menu_buttons_graph_->height + gap);
        const auto isHovering = Math::InRange(mouseX, buttonX, buttonX + menu_buttons_graph_->width) &&
            Math::InRange(mouseY, buttonY, buttonY + menu_buttons_graph_->height);
        menu_buttons_graph_->Draw(
            isHovering ? buttonX - 4 : buttonX,
            buttonY,
            0,
            isHovering ? tileY + 1 : tileY
        );

        if (isHovering && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
        {
            switch (i)
            {
            // ゲームに戻る
            case 0:
                {
                    SetMenuOpen(false);
                    break;
                }
            // はじめから
            case 1:
                {
                    if (const auto stageWorld = dynamic_cast<StageWorld*>(this))
                    {
                        stageWorld->Reload();
                    }
                    break;
                }
            // ステージセレクト
            case 2:
                {
                    Game::instance->ChangeWorldWithTransition<StageSelectWorld>(TransitionMode::Circle);
                    break;
                }
            // ゲームを閉じる
            case 3:
                {
                    SetWindowVisibleFlag(false);
                    Game::shouldShutdown = true;
                    break;
                }
            default:
                break;
            }
        }
    }

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    menu_transition_delta_ += Game::deltaTime;
}

static float fps = 100.0f;
static float duration = 0.5f;
static int gap = 4;

void World::DrawBallistic()
{
    ImGui::Begin("Ballistic");
    ImGui::InputFloat("Frame rate", &fps, 0.1f, 1);
    ImGui::InputFloat("Duration", &duration, 0.1f, 1);
    ImGui::InputInt("Gap", &gap);
    ImGui::End();
    
    if (!player_->CanShot())
    {
        return;
    }

    const auto& dragVector = player_->GetDragVector();

    if (dragVector.Length() <= 0)
    {
        ballistic_->transform.translate = {};
        return;
    }


    // static constexpr float fps = 100.0f;
    static const float frame_sec = 1.0f / fps;
    // static constexpr float duration_sec = 2.25f;
    // static constexpr int steps = 10;
    // static constexpr int gap = 5;
    static const float delta = frame_sec;

    ballistic_->transform.translate = player_->transform.translate.Copy().Add(
        player_->intersectingNormal.Copy().Mul(0.0001));
    ballistic_->velocity = {};

    ballistic_->Reset();

    ballistic_->Update(0);

    auto shotVec = dragVector;
    shotVec.Mul(20.0f);
    // 重力の影響を相殺する
    for (const auto& gravitySource : ballistic_->gravitySources)
    {
        shotVec.Add(gravitySource.Copy().Neg());
    }
    ballistic_->velocity = shotVec;

    ballistic_->UpdateMovement(0);
    ballistic_->PostUpdate(0);

    ballistic_->shouldDraw = true;

    for (int i = 0; i < static_cast<int>(fps * duration); ++i)
    {
        ballistic_->Update(delta);
        ballistic_->UpdateMovement(delta);

        if (gap == 0 || i % gap == 0)
        {
            ballistic_->CreatePoint();
        }

        ballistic_->PostUpdate(delta);
    }

    auto stack = DrawStack();

    // カメラの位置を補正する
    auto originOffset = Vec2(640, 360); // FIXME マジックナンバーをやめる
    originOffset.Div(camera_->zoom);

    auto trans = camera_->transform.translate;
    trans.Mul(-1);
    trans.Add(originOffset);
    stack.Translate(trans);
    stack.Scale(camera_->zoom);
    ballistic_->DrawPoints(&stack);
    ballistic_->shouldDraw = false;
}

int World::NextComponentId()
{
    return next_id_++;
}

bool World::AddComponent(const std::shared_ptr<Component>& component)
{
    if (component_map_.contains(component->GetId()))
    {
        spdlog::warn("Failed to add component. ID {} already exists", component->GetId());
        return false;
    }
    component->world = this;
    component_map_.insert(std::make_pair(component->GetId(), component));
    return true;
}

bool World::RemoveComponent(const int id)
{
    if (!component_map_.contains(id))
    {
        return false;
    }

    component_map_.erase(id);
    return true;
}

std::vector<Component*> World::GetComponents()
{
    std::vector<Component*> components;
    components.reserve(component_map_.size());
    std::ranges::transform(component_map_, std::back_inserter(components), [](auto& pair)
    {
        return pair.second.get();
    });
    return components;
}

Component* World::GetComponent(const int id)
{
    const auto found = component_map_.find(id);
    if (found == component_map_.end())
    {
        return nullptr;
    }
    return found->second.get();
}

CameraComponent& World::GetCamera() const
{
    return *camera_;
}

PlayerComponent* World::GetPlayer() const
{
    return player_.get();
}

std::vector<PhysicsComponent*> World::GetNearbyPhysicsComponents(const Vec2& origin, const float radius) const
{
    std::vector<PhysicsComponent*> nearbyComponents;

    if (radius <= 0)
    {
        return nearbyComponents;
    }

    for (const auto& component : component_map_ | std::views::values)
    {
        const auto componentPosition = component->transform.translate;
        const auto distance = componentPosition.Distance(origin);
        const auto physCompPtr = dynamic_cast<PhysicsComponent*>(component.get());
        if (distance <= radius && physCompPtr)
        {
            nearbyComponents.push_back(physCompPtr);
        }
    }

    return nearbyComponents;
}

WorldType World::GetType() const
{
    throw std::runtime_error("Not implemented");
}

void World::OnCameraMoveWithMouse(CameraComponent* camera)
{
    //
}

void World::OnGoal()
{
}

bool World::CanPlayerShot()
{
    return true;
}

void World::DrawBackground(DrawStack& stack) const
{
}

void World::SetMenuOpen(bool open)
{
    menu_opened_ = open;
    Game::instance->isPaused = open;
    menu_transition_delta_ = 0;
}
