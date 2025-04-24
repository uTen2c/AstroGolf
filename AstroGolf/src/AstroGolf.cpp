// DxLibがShift-JISなので警告が出る
#pragma warning(disable : 4828)

#include "spdlog/spdlog.h"

#include "DxLib.h"
#include "Game.h"
#include "component/SimpleSquareComponent.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

namespace
{
    const auto game = Game();
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);
    SetFullScreenScalingMode(DX_FSSCALINGMODE_NEAREST, TRUE);
    SetGraphMode(1280, 720, 32, 144);
    SetMainWindowText(_T("Astro Golf"));
    ChangeWindowMode(true);
    SetHookWinProc(WndProc);

    if (DxLib_Init() == -1)
    {
        spdlog::error("DxLib initialize failed");
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui_ImplWin32_Init(GetMainWindowHandle());
    ImGui_ImplDX11_Init((ID3D11Device*)GetUseDirect3D11Device(), (ID3D11DeviceContext*)GetUseDirect3D11DeviceContext());

    const auto demo1 = std::make_shared<SimpleSquareComponent>(game.GetWorld().NextComponentId(), Pos({100, 300}, 0));
    const auto demo2 = std::make_shared<SimpleSquareComponent>(game.GetWorld().NextComponentId(), Pos({150, 300}, 0));
    game.GetWorld().AddComponent(demo1);
    game.GetWorld().AddComponent(demo2);

    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        SetDrawScreen(DX_SCREEN_BACK);
        SetDrawAreaFull();
        ClearDrawScreen();

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();

        auto move = Vector2d(0, 0);
        if (CheckHitKey(KEY_INPUT_A))
        {
            spdlog::info("A");
            move = move.AddX(-1);
        }
        if (CheckHitKey(KEY_INPUT_D))
        {
            spdlog::info("D");
            move = move.AddX(1);
        }

        const auto camera = &game.GetWorld().GetCamera();
        camera->SetPos(camera->GetPos().SetPoint(camera->GetPos().GetPoint().Add(move)));
        ImGui::Begin("Pos");
        ImGui::Text("%f, %f", camera->GetPos().GetPoint().GetX(), camera->GetPos().GetPoint().GetY());
        ImGui::End();

        game.GetWorld().Draw();

        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        ScreenFlip();
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    DxLib_End();

    return 0;
}
