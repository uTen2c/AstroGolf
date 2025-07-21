#pragma once
#include <string>

#include "../Component.h"
#include "../../graph/Graph.h"

class StageSelectComponent final : public Component
{
    int font_handle_;
    int current_center_index_ = 0;
    int focused_index_ = 0;
    int last_focused_index_ = 0;
    int mouse_hovering_index_ = -1;
    bool mouse_clicked_ = false;
    int clicked_index_ = -1;

public:
    explicit StageSelectComponent(int id);
    ~StageSelectComponent() override;

    void Update(float delta) override;
    void Draw(DrawStack* stack) override;

    [[nodiscard]] int GetFocusedIndex() const;

private:
    void DrawButton(const float& x, const float& y, const std::string& label, const bool& selected, const int& stars) const;
    static int CheckMoveKey(float deltaTime);

    static void OnSelect(int index);
};
