// UnitTest.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "UnitTest.h"
#include "../UEngine/UEngine.h"
#include "../WinApplication/WinConsole.h"
#include "../WinApplication/WinMemoryLeak.h"
#include "../Utility/UThreadSync.h"

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
    auto renderer = UEngine::DXRenderer::Get();
    UEngine::DX_RENDERER_DESC rendererDesc = UEngine::DXRenderer::CreateDefaultInitDesc();
    {
        rendererDesc.IsDebuggable = true;
        rendererDesc.EnableAntialisedLine = true;
        rendererDesc.EnableBlendState = true;
        rendererDesc.EnableDepthStencil = true;
        rendererDesc.EnableMultisampling = true;
        rendererDesc.MultisampleDesc = { 4, 0 };
        renderer->Init(app->GetHandler(), &rendererDesc);
    }

    // View & Object Creation
    RECT windowSize;
    app->GetClientSize(&windowSize);
    UEngine::DXView* view = UEngine::DXView::Instantiate
    (
        renderer, 
        windowSize.right - windowSize.left, 
        windowSize.bottom - windowSize.top, 
        rendererDesc.EnableDepthStencil, 
        rendererDesc.MultisampleDesc
    );
    UEngine::DXRenderObject* renderObj;
    {
        renderObj = UEngine::DXGeometryFigurePrefab::CreateCircle();
        view->AddRenderObject(renderObj);
    }

    auto threadPool = UEngine::Utility::UThreadSync::Get();
    threadPool->Init();

    auto returnedValue = app->UpdateLoop([&]() 
    {
        UEngine::Utility::UTime::Get()->Throttle(200);

        // constant buffers mapping
        view->UpdateConstantBuffers();
        renderer->UpdateConstantBuffers();

        // rendering in different thread
        threadPool->AddTask([&]() {
            view->Begin();
            view->End(renderer->GetImmediateDeviceContext());

            renderer->Begin(DirectX::Colors::Transparent);
            renderer->GetImmediateDeviceContext()->PSSetShaderResources(0, 1, view->GetAddressOfViewResource());
            renderer->End();
        }); // render target view

        // update in main thread
        {
            UEngine::WinConsole::ResetCursorPos();
            std::cout << "\t\t" << std::endl;
            std::cout << "\t\t" << std::endl;
            UEngine::WinConsole::ResetCursorPos();

            std::cout << UEngine::Utility::UTime::Get()->FramePerSecond() << std::endl;
            std::cout << UEngine::Utility::UTime::Get()->DeltaTime() << std::endl;
        }
        
        
    });

    UEngine::DXRenderObject::Release(&renderObj);
    UEngine::DXView::Release(&view);

    return returnedValue;
}
