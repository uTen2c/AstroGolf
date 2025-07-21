#pragma once
#include <DxLib.h>

#include "PhysicsComponent.h"

class PlayerComponent final : public PhysicsComponent
{
    bool landing_ = true;
    bool valid_shot_angle_ = true;
    Transform shot_start_transform_ = {};
    std::deque<Vec2> trails_ = {};
    int trail_screen_;
    bool should_trails_ = false;
    int shot_count_ = 0;
    float last_move_speed_ = 0;
    Vec2 last_move_speed_vec_;
    int last_hit_sound_played_at_ = GetNowCount();

public:
    Vec2 intersectingNormal;
    float radius = 12;
    bool isDragging = false;

    explicit PlayerComponent(int id);
    ~PlayerComponent() override;

    void Update(float deltaTime) override;
    void UpdateMovement(float deltaTime) override;
    void Draw(DrawStack* stack) override;

    void Respawn();

    [[nodiscard]] Vec2 GetDragVector() const;
    [[nodiscard]] bool CanShot() const;
    [[nodiscard]] int GetShotCount() const;

private:
    Vec2 drag_vector_;
    Vec2 screen_pos_;

    void UpdateShot();
    void UpdateDebugMove();

    void DrawTrail(DrawStack* stack);

    static void PlayShotSound();

protected:
    void OnCollide(PhysicsComponent* other, const IntersectingResult& result) override;
};
