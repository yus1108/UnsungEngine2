// UnitTest.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "UnitTest.h"
#include "../UEngine/UEngine.h"
#include "../WinApplication/WinConsole.h"
#include "../WinApplication/WinMemoryLeak.h"

#define MAX_LOADSTRING 100

// Global Variables:
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
    UEngine::WinMemoryLeak::Detect();

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_UNITTEST, szWindowClass, MAX_LOADSTRING);

    UEngine::GameObject* gameObject = UEngine::GameObject::Instantiate();

    // Window Application
    auto app = UEngine::WinApplication::Get();
    {
        WNDCLASSEXW Wcex;

        Wcex.cbSize = sizeof(WNDCLASSEX);

        Wcex.style = CS_HREDRAW | CS_VREDRAW;
        Wcex.lpfnWndProc = nullptr;
        Wcex.cbClsExtra = 0;
        Wcex.cbWndExtra = 0;
        Wcex.hInstance = hInstance;
        Wcex.hIcon = nullptr;
        Wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        Wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        Wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_UNITTEST);
        Wcex.lpszClassName = szWindowClass;
        Wcex.hIconSm = LoadIcon(Wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

        UEngine::WINDOWS_APPLICATION_DESC desc;
        ZeroMemory(&desc, sizeof(desc));

        desc.HInstance = hInstance;
        desc.HasTitleBar = true;
        desc.InitUTime = true;
        desc.InitWinInput = true;
        desc.NCmdShow = nCmdShow;
        desc.TitleName = szTitle;
        desc.WindowSize = { 800, 600 };
        desc.Wcex = &Wcex;

        app->Create(desc);
    }

    // DXRenderer
    auto renderer = UEngine::DXRenderer::DXRenderer::Get();
    UEngine::DX_RENDERER_DESC rendererDesc = UEngine::DXRenderer::DXRenderer::CreateDefaultInitDesc();
    {
        rendererDesc.IsDebuggable = true;
        rendererDesc.EnableAntialisedLine = false;
        rendererDesc.EnableBlendState = true;
        rendererDesc.EnableDepthStencil = true;
        rendererDesc.EnableMultisampling = false;
        rendererDesc.MultisampleDesc = { 1, 0 };
        renderer->Init(app->GetHandler(), &rendererDesc);
    }

    auto gameState = UEngine::GameState::Get();
    gameState->Init(app, renderer);
    gameState->LoadScene("");

    auto returnedValue = app->UpdateLoop([&]() 
    {
        UEngine::Utility::UTime::Get()->Throttle(200);
        gameState->Update();
    });

    gameState->Release();

    return returnedValue;
}
