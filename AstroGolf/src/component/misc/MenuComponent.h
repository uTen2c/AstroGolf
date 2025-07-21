#pragma once
#include "../Component.h"
#include "../../graph/Graph.h"

class MenuComponent final : public Component
{
    bool menu_opened_ = false;
    bool menu_button_clicked_ = false;
    int clicked_index_ = -1;
    float menu_transition_delta_ = 0;
    int last_hovering_index_ = -1;

public:
    explicit MenuComponent(int id);

    void SetMenuOpen(bool open);

    void Update(float delta) override;
    void Draw(DrawStack* stack) override;

private:
    void DrawMenuButton();
    void DrawMenu();
    void DrawMenuItem(const float x, const float y, const int buttonIndex, const std::function<void()>& onClick);
};
