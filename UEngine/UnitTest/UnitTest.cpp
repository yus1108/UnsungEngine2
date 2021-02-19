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
#include "../DXRenderer/dxrframework.h"

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

    UEngine::DXRenderingDesc rendererDesc = UEngine::DXRenderer::CreateDefaultInitDesc();
    rendererDesc.IsDebuggable = true;
    auto renderer = UEngine::DXRenderer::Get();
    renderer->Init(app->GetHandler(), false, false, &rendererDesc);

    UEngine::DXRenderObject* renderObj;
    {
        // Shader
        UEngine::DXRenderingDesc rsDesc = UEngine::DXRenderingDesc();
        auto default_ssDesc = renderer->SSCreateDesc();
        auto default_bsDesc = renderer->BSCreateDesc();
        auto default_shader = UEngine::DXShader::Instantiate
        (
            renderer,
            "../_Shaders/DefaultVS.hlsl",
            "../_Shaders/ColorPS.hlsl",
            true,
            &rsDesc.RasterizerStateDesc,
            &default_ssDesc,
            &default_bsDesc
        );

        // RenderMesh
        UEngine::SIMPLE_VERTEX vertices[] =
        {
            UEngine::SIMPLE_VERTEX{DirectX::XMFLOAT3{-0.5f, -0.5f, 0}, DirectX::XMFLOAT2{0, 1}},
            UEngine::SIMPLE_VERTEX{DirectX::XMFLOAT3{-0.5f, 0.5f, 0}, DirectX::XMFLOAT2{0, 0}},
            UEngine::SIMPLE_VERTEX{DirectX::XMFLOAT3{0.5f, -0.5f, 0}, DirectX::XMFLOAT2{1, 1}},
            UEngine::SIMPLE_VERTEX{DirectX::XMFLOAT3{0.5f, 0.5f, 0}, DirectX::XMFLOAT2{1, 0}},
        };
        unsigned indices[] = { 0, 1, 2, 2, 1, 3 };
        auto default_renderMesh = UEngine::DXRenderMesh::Instantiate<UEngine::SIMPLE_VERTEX>(renderer->GetDevice(), &vertices[0], ARRAYSIZE(vertices), indices, ARRAYSIZE(indices));

        renderObj = UEngine::DXRenderObject::Instantiate(default_renderMesh, default_shader, true);

        DirectX::XMFLOAT4 color{ 1,1,1,1 };
        renderObj->AddConstantBuffer(renderer, "Color", sizeof(DirectX::XMFLOAT4), UENGINE_DXSHADERTYPE_PIXEL_SHADER);
        renderObj->UpdateConstantBuffer(renderer->GetImmediateDeviceContext(), "Color", &color, sizeof(DirectX::XMFLOAT4));
    }
    
    UEngine::DXRenderObject* renderObj2;
    {
        // Shader2
        UEngine::DXRenderingDesc rsDesc = UEngine::DXRenderingDesc();
        auto default_ssDesc = renderer->SSCreateDesc();
        auto default_bsDesc = renderer->BSCreateDesc();
        auto default_shader = UEngine::DXShader::Instantiate
        (
            renderer,
            "../_Shaders/DefaultVS.hlsl",
            "../_Shaders/ColorPS.hlsl",
            true,
            &rsDesc.RasterizerStateDesc,
            &default_ssDesc,
            &default_bsDesc
        );

        // RenderMesh2
        UEngine::SIMPLE_VERTEX vertices[] =
        {
            UEngine::SIMPLE_VERTEX{DirectX::XMFLOAT3{-0.2, -0.5f, 0}, DirectX::XMFLOAT2{0, 1}},
            UEngine::SIMPLE_VERTEX{DirectX::XMFLOAT3{-0.2, 0.5f, 0}, DirectX::XMFLOAT2{0, 0}},
            UEngine::SIMPLE_VERTEX{DirectX::XMFLOAT3{0.5f, -0.5f, 0}, DirectX::XMFLOAT2{1, 1}},
            UEngine::SIMPLE_VERTEX{DirectX::XMFLOAT3{0.5f, 0.5f, 0}, DirectX::XMFLOAT2{1, 0}},
        };
        auto default_renderMesh = UEngine::DXRenderMesh::Instantiate<UEngine::SIMPLE_VERTEX>(renderer->GetDevice(), &vertices[0], ARRAYSIZE(vertices));

        renderObj2 = UEngine::DXRenderObject::Instantiate(default_renderMesh, default_shader, true);

        DirectX::XMFLOAT4 color{ 0,0.5f,0,1 };
        renderObj2->AddConstantBuffer(renderer, "Color", sizeof(DirectX::XMFLOAT4), UENGINE_DXSHADERTYPE_PIXEL_SHADER);
        renderObj2->UpdateConstantBuffer(renderer->GetImmediateDeviceContext(), "Color", &color, sizeof(DirectX::XMFLOAT4));
    }

    UEngine::DXView* view = UEngine::DXView::Instantiate(renderer, 400, 400);
    view->AddRenderObject(renderObj);
    view->AddRenderObject(renderObj2);

    auto returnedValue = app->UpdateLoop([&]() 
    {
        UEngine::Utility::UTime::Get()->Throttle(200);

        UEngine::WinConsole::ResetCursorPos();
        std::cout << "\t\t" << std::endl;
        std::cout << "\t\t" << std::endl;
        UEngine::WinConsole::ResetCursorPos();

        std::cout << UEngine::Utility::UTime::Get()->FramePerSecond() << std::endl;
        std::cout << UEngine::Utility::UTime::Get()->DeltaTime() << std::endl;

        view->Set();
        view->Begin();
        view->End(renderer->GetImmediateDeviceContext());

        renderer->Begin(DirectX::Colors::Gray);
        
        renderObj->GetConstantBuffer("Color")->Set(renderer->GetImmediateDeviceContext());
        /*renderer->GetImmediateDeviceContext()->ResolveSubresource
        (
            (ID3D11Resource*)newRender->OutputTexture2D.Get(), 
            D3D11CalcSubresource(0, 0, 1),
            (ID3D11Resource*)newRender->RenderTargetViewTexture2D.Get(), 
            D3D11CalcSubresource(0, 0, 1), 
            DXGI_FORMAT_R32G32B32A32_FLOAT
        );*/
       /* ID3D11ShaderResourceView* baseTexture[]
        {
            (ID3D11ShaderResourceView*)newRender->OutputShaderResourceView.Get()
        };*/
        renderer->GetImmediateDeviceContext()->PSSetShaderResources(0, 1, view->GetAddressOfViewResource());

        renderer->End();
    });

    UEngine::DXRenderObject::Release(&renderObj);
    UEngine::DXRenderObject::Release(&renderObj2);
    UEngine::DXView::Release(&view);

    return returnedValue;
}
