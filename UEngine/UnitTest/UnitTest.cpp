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
    wcex.lpfnWndProc = nullptr;
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
    desc.HasTitleBar = true;
    desc.InitUTime = true;
    desc.InitWinInput = true;
    desc.NCmdShow = nCmdShow;
    desc.TitleName = szTitle;
    desc.WindowSize = { 800, 600 };
    desc.wcex = &wcex;

    auto app = UEngine::WinApplication::Get();
    app->Create(desc);

    auto renderer = UEngine::DXRenderer::Get();
    renderer->Init(app->GetHandler());

    auto shader = UEngine::DXShader::Instantiate(renderer->GetDevice(), "../_Shaders/DefaultVS.hlsl", "../_Shaders/DefaultPS.hlsl");

    std::vector<UEngine::SIMPLE_VERTEX> vertices
    {
       UEngine::SIMPLE_VERTEX{DirectX::XMFLOAT3{-0.5f, -0.5f, 0}},
       UEngine::SIMPLE_VERTEX{DirectX::XMFLOAT3{-0.5f, 0.5f, 0}},
       UEngine::SIMPLE_VERTEX{DirectX::XMFLOAT3{0.5f, -0.5f, 0}},
    };
    std::vector<unsigned> indices{ 0 ,1, 2 };
    auto renderMesh = UEngine::RenderMesh<UEngine::SIMPLE_VERTEX>::Instantiate(renderer->GetDevice(), vertices, indices);

    auto returnedValue = app->UpdateLoop([&]() 
    {
        UEngine::Utility::UTime::Get()->Throttle(200);

        UEngine::WinConsole::ResetCursorPos();
        std::cout << "\t\t" << std::endl;
        std::cout << "\t\t" << std::endl;
        UEngine::WinConsole::ResetCursorPos();

        std::cout << UEngine::Utility::UTime::Get()->FramePerSecond() << std::endl;
        std::cout << UEngine::Utility::UTime::Get()->DeltaTime() << std::endl;

        renderer->Begin();
        renderMesh->Render(renderer->GetImmediateDeviceContext());
        shader->Render(renderer->GetImmediateDeviceContext());
        renderer->GetImmediateDeviceContext()->DrawIndexed(renderMesh->GetIndicesCount(), 0, 0);
        renderer->End();
        
    });

    UEngine::DXShader::Release(&shader);
    UEngine::RenderMesh<UEngine::SIMPLE_VERTEX>::Release(&renderMesh);

    return returnedValue;
}
