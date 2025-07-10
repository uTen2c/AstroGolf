#include "World.h"

#include <algorithm>
#include <imgui.h>
#include <ranges>
#include <spdlog/spdlog.h>

#include "../Game.h"
#include "../game/StageManager.h"
#include "../graph/Graph.h"
#include "../math/Math.h"

World::World() = default;

void World::Init()
{
    spdlog::info("World init");
    camera_ = std::make_shared<CameraComponent>(NextComponentId());
    player_ = std::make_shared<PlayerComponent>(NextComponentId());
    ballistic_ = std::make_shared<BallisticComponent>(NextComponentId());

    AddComponent(camera_);
    AddComponent(player_);
    AddComponent(ballistic_);

    if (menuEnabled)
    {
        menu_ = std::make_shared<MenuComponent>(NextComponentId());
        AddComponent(menu_);
    }
}

void World::Draw()
{
    auto stack = CreateDrawStack();

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

    PostDraw(stack);
}

void World::Update(const float& deltaTime)
{
    if (!add_queue_.empty())
    {
        for (const auto& component : add_queue_)
        {
            component->world = this;
            component_map_.insert(std::make_pair(component->GetId(), component));
        }

        for (const auto& component : add_queue_)
        {
            component->Init();
        }

        add_queue_.clear();
    }

    const auto components = GetComponents();
    for (const auto component : components)
    {
        if (dynamic_cast<BallisticComponent*>(component))
        {
            continue;
        }
        if (!Game::instance->isPaused || component->updateWhenPaused)
        {
            component->Update(deltaTime);
        }
    }
    for (const auto component : components)
    {
        if (component && dynamic_cast<BallisticComponent*>(component))
        {
            continue;
        }
        if (
            const auto physComp = dynamic_cast<PhysicsComponent*>(component);
            physComp && (!Game::instance->isPaused || component->updateWhenPaused))
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
        if (!Game::instance->isPaused || component->updateWhenPaused)
        {
            component->PostUpdate(deltaTime);
        }
    }

    for (int id : remove_queue_)
    {
        component_map_.erase(id);
    }

    remove_queue_.clear();
}

static float fps = 100.0f;
static float duration = 0.5f;
static int gap = 4;

void World::DrawBallistic()
{
    if (Game::debugEnabled)
    {
        ImGui::Begin("Ballistic");
        ImGui::InputFloat("Frame rate", &fps, 0.1f, 1);
        ImGui::InputFloat("Duration", &duration, 0.1f, 1);
        ImGui::InputInt("Gap", &gap);
        ImGui::End();
    }

    ballistic_->transform.translate = player_->transform.translate.Copy().Add(
        player_->intersectingNormal.Copy().Mul(0.0001f));
    ballistic_->velocity = {};

    ballistic_->Reset();

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


    static const float frame_sec = 1.0f / fps;
    static const float delta = frame_sec;

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
    add_queue_.emplace_back(component);
    // component->world = this;
    // component_map_.insert(std::make_pair(component->GetId(), component));
    // component->Init();
    return true;
}

bool World::RemoveComponent(const int id)
{
    if (!component_map_.contains(id))
    {
        return false;
    }

    remove_queue_.emplace_back(id);
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

void World::OnShot()
{
}

void World::OnGoal()
{
}

bool World::CanPlayerShot()
{
    return true;
}

void World::DrawBackground(DrawStack& stack)
{
}

void World::PostDraw(DrawStack& stack)
{
}

Vec2 World::GetWorldPos(const Vec2& screenPos) const
{
    // カメラの位置を補正する
    Vec2 pos;
    pos += camera_->transform.translate;
    pos += (screenPos + Vec2(-640, -360)) / camera_->zoom;
    return pos;
}

void World::SetMenuOpen(const bool open) const
{
    if (menu_)
    {
        menu_->SetMenuOpen(open);
    }
}

DrawStack World::CreateDrawStack() const
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

    return stack;
}
