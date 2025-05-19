#include "spdlog/spdlog.h"

#include "DxLib.h"
#include "Game.h"
#include "component/SimpleSquareComponent.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "component/BoxComponent.h"
#include "component/CircleComponent.h"
#include "component/PlanetComponent.h"
#include "component/RotatableBoxComponent.h"
#include "component/planet/DemoPlanetComponent.h"
#include "math/Math.h"
#include "world/DemoWorld.h"

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
    SetWindowSizeChangeEnableFlag(true);
    SetHookWinProc(WndProc);
    // SetWindowStyleMode(7);
    SetWaitVSyncFlag(false);
    SetBackgroundColor(20, 29, 39);

    if (DxLib_Init() == -1)
    {
        spdlog::error("DxLib initialize failed");
        return -1;
    }

    const auto game = std::make_unique<Game>();
    game->ChangeWorld<DemoWorld>();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui_ImplWin32_Init(GetMainWindowHandle());

    // ReSharper disable once CppCStyleCast
    ImGui_ImplDX11_Init((ID3D11Device*)GetUseDirect3D11Device(), (ID3D11DeviceContext*)GetUseDirect3D11DeviceContext()); // NOLINT(clang-diagnostic-cast-qual)

    // FPS計測関係の初期化
    auto fpsCheckTime = GetNowHiPerformanceCount();
    auto fpsCounter = 0;
    auto time = GetNowHiPerformanceCount();

    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();

        ScreenFlip();

        WaitTimer(1000 / 200); // 144FPS付近に固定する

        SetDrawScreen(DX_SCREEN_BACK);
        SetDrawAreaFull();
        ClearDrawScreen();

        // 現在のシステム時間を取得
        const long long nowTime = GetNowHiPerformanceCount();

        // 前回取得した時間からの経過時間を秒に変換してセット
        // ( GetNowHiPerformanceCount で取得できる値はマイクロ秒単位なので 1000000 で割ることで秒単位になる )
        Game::deltaTime = (nowTime - time) / 1000000.0f;
        
        game->Update();

        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        DrawFormatString(0, 0, GetColor(255, 255, 255), "FPS:%d", Game::fps);

        // 今回取得した時間を保存
        time = nowTime;

        // FPS関係の処理( 1秒経過する間に実行されたメインループの回数を FPS とする )
        fpsCounter++;
        if (nowTime - fpsCheckTime > 1000000)
        {
            Game::fps = fpsCounter;
            fpsCounter = 0;
            fpsCheckTime = nowTime;
        }
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    DxLib_End();

    return 0;
}
