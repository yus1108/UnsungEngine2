// UnitTest.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "UnitTest.h"
#include "../WinApplication/WinApplication.h"
#include "../WinApplication/WinInput.h"
#include "../WinApplication/WinConsole.h"
#include "../WinApplication/WinMemoryLeak.h"
#include "../Utility/UMath.h"
#include "../Utility/UTime.h"
#include "../DXRenderer/DXRenderer.h"

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

    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = nullptr;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_UNITTEST);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    UEngine::WINDOWS_APPLICATION_DESC desc;
    ZeroMemory(&desc, sizeof(desc));

    desc.HInstance = hInstance;
    desc.TitleName = szTitle;
    desc.NCmdShow = nCmdShow;
    desc.WindowSize = { 800, 600 };
    desc.wcex = &wcex;
    desc.HasTitleBar = true;

    auto app = UEngine::WinApplication::Get();
    app->Create(hInstance);

    auto renderer = UEngine::DXRenderer::Get();
    renderer->Init(app->GetHandler());

    auto returnedValue = app->UpdateLoop([&]() 
    {
        //system("cls");
        UEngine::WinConsole::ResetCursorPos();
        std::cout << "\t\t" << std::endl;
        std::cout << "\t\t" << std::endl;
        std::cout << "\t\t" << std::endl;
        std::cout << "\t\t" << std::endl;
        std::cout << "\t\t" << std::endl;
        std::cout << "\t\t" << std::endl;
        std::cout << "\t\t" << std::endl;
        std::cout << "\t\t" << std::endl;
        std::cout << "\t\t" << std::endl;
        std::cout << "\t\t" << std::endl;
        UEngine::WinConsole::ResetCursorPos();
        RECT clientSize, windowSize;
        app->GetClientSize(&clientSize);
        GetWindowRect(app->GetHandler(), &windowSize);
        UEngine::Utility::UTime::Get()->Signal();
        std::cout << UEngine::Utility::UTime::Get()->FramePerSecond() << std::endl;
        std::cout << UEngine::Utility::UTime::Get()->DeltaTime() << std::endl;
        std::cout << UEngine::Utility::UMath::ConvertPixelToNDC(UEngine::WinInput::Get()->GetMousePos(), app->GetHandler()).x << std::endl;
        std::cout << UEngine::Utility::UMath::ConvertPixelToNDC(UEngine::WinInput::Get()->GetMousePos(), app->GetHandler()).y << std::endl;
        std::cout << UEngine::WinInput::Get()->GetMousePos().x << std::endl;
        std::cout << UEngine::WinInput::Get()->GetMousePos().y << std::endl;
        std::cout << clientSize.right - clientSize.left << std::endl;
        std::cout << clientSize.bottom - clientSize.top << std::endl;
        std::cout << windowSize.right - windowSize.left << std::endl;
        std::cout << windowSize.bottom - windowSize.top << std::endl;
        UEngine::Utility::UTime::Get()->Throttle(200);
    });

    return returnedValue;
}


void DefaultWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_MOUSEMOVE:
        UEngine::WinInput::Get()->SetMousePos(UEngine::Definition::Coordinate2D{ (float)LOWORD(lParam), (float)HIWORD(lParam) });
        break;
    case WM_KEYDOWN:
        UEngine::WinInput::Get()->SetKeyPress(true, wParam);
        break;
    case WM_KEYUP:
        UEngine::WinInput::Get()->SetKeyPress(false, wParam);
        break;
    case WM_LBUTTONDOWN:
        UEngine::WinInput::Get()->SetMousePress(true, UEngine::WinInput::MouseType::LEFT);
        break;
    case WM_LBUTTONUP:
        UEngine::WinInput::Get()->SetMousePress(false, UEngine::WinInput::MouseType::LEFT);
        break;
    case WM_RBUTTONDOWN:
        UEngine::WinInput::Get()->SetMousePress(true, UEngine::WinInput::MouseType::RIGHT);
        break;
    case WM_RBUTTONUP:
        UEngine::WinInput::Get()->SetMousePress(false, UEngine::WinInput::MouseType::RIGHT);
        break;
    case WM_MOUSEWHEEL:
        UEngine::WinInput::Get()->SetMousePress(true, UEngine::WinInput::MouseType::SCROLL);
        UEngine::WinInput::Get()->scroll = (int)wParam;
        break;
    case WM_TOUCH:
        break;
    case WM_MBUTTONDOWN:
        UEngine::WinInput::Get()->SetMousePress(true, UEngine::WinInput::MouseType::MIDDLE);
        break;
    case WM_MBUTTONUP:
        UEngine::WinInput::Get()->SetMousePress(false, UEngine::WinInput::MouseType::MIDDLE);
        break;
    }
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    DefaultWndProc(hWnd, message, wParam, lParam);
    //어떤 메세지가 발생되었는가를 통해 처리할 조건문
    if (message == WM_DESTROY || message == WM_CLOSE)
    {
        PostQuitMessage(0);
        return (DefWindowProc(hWnd, message, wParam, lParam));
    }

    return (DefWindowProc(hWnd, message, wParam, lParam));
}

