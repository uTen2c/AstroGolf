#pragma once
#include <string>

#include "../Component.h"

class StageSelectComponent final : public Component
{
    int current_index_ = 0;
    inline static bool last_down_pressed_ = false;
    inline static bool last_up_pressed_ = false;
    inline static float key_check_duration_ = 0.f;
    inline static bool key_check_repeat_ = false;

public:
    explicit StageSelectComponent(int id);

    void Update(float delta) override;
    void Draw(DrawStack* stack) override;

private:
    static void DrawButton(const float& x, const float& y, const float& scale, const std::string& label, const bool& selected);
    static int CheckMoveKey(float deltaTime);
};
