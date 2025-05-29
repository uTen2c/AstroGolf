#include "TestSquareComponent.h"

#include <DxLib.h>
#include <imgui.h>

TestSquareComponent::TestSquareComponent(const int id)
    : Component(id),
      graph_(std::make_unique<Graph>("test_square.png", 200, 200))
{
}

void TestSquareComponent::Update(float delta)
{
    ImGui::Begin("Test square");
    ImGui::InputFloat("Scale X", &scale_x_, 0.1f, 1.0f);
    ImGui::InputFloat("Scale Y", &scale_y_, 0.1f, 1.0f);
    ImGui::End();
}

void TestSquareComponent::Draw(DrawStack* stack)
{
    stack->Push();
    ApplyDrawStack(stack);
    graph_->Draw(*stack);

    const auto & screenPos = stack->GetScreenPos();
    DrawCircleAA(screenPos.x, screenPos.y, 4, 8, GetColor(255, 0, 0));
    stack->Pop();
}
