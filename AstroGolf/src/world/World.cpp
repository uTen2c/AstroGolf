#include "World.h"

#include <algorithm>
#include <DxLib.h>
#include <ranges>
#include <spdlog/spdlog.h>

#include "StageSelectWorld.h"
#include "../Game.h"
#include "../graph/Graph.h"
#include "../math/Math.h"

World::World()
{
    spdlog::info("World init");

    camera_ = std::make_shared<CameraComponent>(NextComponentId());
    player_ = std::make_shared<PlayerComponent>(NextComponentId());
    menu_button_graph_ = std::make_unique<Graph>("menu_button.png", 72, 72);
    menu_background_graph_ = std::make_unique<Graph>("menu/menu_background.png", 1024, 720);
    menu_buttons_graph_ = std::make_unique<Graph>("menu/buttons.png", 512, 70);

    AddComponent(camera_);
    AddComponent(player_);
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

    DrawUi();
}

void World::Update(const float& deltaTime)
{
    const auto components = GetComponents();
    for (const auto component : components)
    {
        component->Update(deltaTime);
    }
    for (const auto component : components)
    {
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

    if (!menu_opened_)
    {
        return;
    }

    // Esc押したら閉じる
    if (CheckHitKey(KEY_INPUT_ESCAPE) != 0)
    {
        SetMenuOpen(false);
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
                    Game::shouldShutdown = true;
                    break;
                }
            default:
                break;
            }
        }
    }
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

void World::DrawBackground(DrawStack& stack) const
{
}

void World::SetMenuOpen(bool open)
{
    menu_opened_ = open;
    Game::instance->isPaused = open;
}
