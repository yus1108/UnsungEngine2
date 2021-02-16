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

    auto app = UEngine::WinApplication::Get();
    app->Create(hInstance, nCmdShow, { 800, 600 }, false, szTitle, szWindowClass);

    auto renderer = UEngine::DXRenderer::Get();
    renderer->Init();

    auto returnedValue = app->UpdateLoop([&]() 
    {
        //system("cls");
        UEngine::WinConsole::ResetCursorPos();
        std::cout << "\t\t" << std::endl;
        std::cout << "\t\t" << std::endl;
        UEngine::WinConsole::ResetCursorPos();

        UEngine::Utility::UTime::Get()->Signal();
        std::cout << UEngine::Utility::UTime::Get()->FramePerSecond() << std::endl;
        std::cout << UEngine::Utility::UTime::Get()->DeltaTime() << std::endl;
        //std::cout << UEngine::Utility::UMath::ConvertPixelToNDC(UEngine::WinInput::Get()->GetMousePos(), app->GetHandler()).x << std::endl;
        //std::cout << UEngine::Utility::UMath::ConvertPixelToNDC(UEngine::WinInput::Get()->GetMousePos(), app->GetHandler()).y << std::endl;
        UEngine::Utility::UTime::Get()->Throttle(200);
    });

    return returnedValue;
}