#include "spdlog/spdlog.h"

#include "DxLib.h"
#include "Game.h"
#include "component/SimpleSquareComponent.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "component/BoxComponent.h"

namespace
{
    const auto game = Game();
}

// ReSharper disable once CppInconsistentNaming
LRESULT CALLBACK WndProc(const HWND hwnd, const UINT msg, const WPARAM w_param, const LPARAM l_param)
{
    extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, w_param, l_param))
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

    // ReSharper disable once CppCStyleCast
    ImGui_ImplDX11_Init((ID3D11Device*)GetUseDirect3D11Device(), (ID3D11DeviceContext*)GetUseDirect3D11DeviceContext()); // NOLINT(clang-diagnostic-cast-qual)

    const auto demo1 = std::make_shared<SimpleSquareComponent>(game.GetWorld().NextComponentId());
    demo1->transform.translate = {100, 300};

    const auto demo2 = std::make_shared<SimpleSquareComponent>(game.GetWorld().NextComponentId());
    demo2->transform.translate = {150, 300};

    const auto box = std::make_shared<BoxComponent>(game.GetWorld().NextComponentId(), 200, 20);
    box->transform.translate = {500, 500};

    game.GetWorld().AddComponent(demo1);
    game.GetWorld().AddComponent(demo2);
    game.GetWorld().AddComponent(box);

    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        SetDrawScreen(DX_SCREEN_BACK);
        SetDrawAreaFull();
        ClearDrawScreen();

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();

        auto move = Vec2(0, 0);
        if (CheckHitKey(KEY_INPUT_A))
        {
            spdlog::info("A");
            move.x -= 1;
        }
        if (CheckHitKey(KEY_INPUT_D))
        {
            spdlog::info("D");
            move.x += 1;
        }

        const auto camera = &game.GetWorld().GetCamera();
        camera->translate.x += move.x;
        ImGui::Begin("Pos");
        ImGui::Text("%f, %f", camera->translate.x, camera->translate.y);
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
