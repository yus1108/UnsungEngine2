// UnitTest.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "UnitTest.h"
#include "ScriptComponent.h"

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
    UEngine::DXRenderer::RENDERER_DESC rendererDesc = UEngine::DXRenderer::CreateDefaultInitDesc();
    {
        rendererDesc.IsDebuggable = true;
        rendererDesc.EnableAntialisedLine = false;
        rendererDesc.EnableBlendState = true;
        rendererDesc.EnableDepthStencil = true;
        rendererDesc.EnableMultisampling = true;
        rendererDesc.MultisampleDesc = { 4, 0 };
        renderer->Init(app->GetHandler(), &rendererDesc);
    }

    auto gameState = UEngine::GameState::Get();
    gameState->Init(app, renderer);
    srand((unsigned)time(nullptr));
    std::vector<SpatialPartitioning::Collider*> colliders;
        // TODO: Place code here.
    {
        // basic load
        using namespace UEngine;

        GameObject* mainCamera = GameObject::Instantiate();
        mainCamera->AddComponent<Transform>();
        auto cameraScript = mainCamera->AddComponent<Camera>();
        cameraScript->viewWidth = 80;
        cameraScript->viewHeight = 60;
        gameState->AddObject(mainCamera);

        for (size_t i = 0; i < 20; i++)
        {
            GameObject* circle = GameObject::Instantiate();
            circle->AddComponent<Transform>();
            circle->AddComponent<RenderComponent>()->Load("rectangle", "basicTexture");
            circle->AddComponent<Material>()->Load(L"../_Assets/Textures/football-157930_640.png");
            auto script = circle->AddComponent<ScriptComponent>();
            gameState->AddObject(circle);
            auto collider = new SpatialPartitioning::Collider;
            collider->gameObject = circle;
            script->collider = collider;
            colliders.push_back(collider);
        }
    }

    float counter = 0.0f;
    float timestep = 0.02f;
    float deltatime = 0.0f;
    auto returnedValue = app->UpdateLoop([&]() 
    {
        UEngine::Utility::UTime::Get()->Throttle(200);
        deltatime = UEngine::Utility::UTime::Get()->DeltaTimeF();
        counter += deltatime;

        gameState->Update();
        if (counter > UEngine::Math::Clamp(deltatime, 0.02f, 0.1f))
        {
            Math::Physics2D::AABB worldAABB = colliders[0]->gameObject->GetComponent<ScriptComponent>()->aabb;
            colliders[0]->aabb = worldAABB;
            for (size_t i = 1; i < colliders.size(); i++)
            {
                colliders[i]->aabb = colliders[i]->gameObject->GetComponent<ScriptComponent>()->aabb;
                worldAABB = ScriptComponent::sp.EnlargeGrid(worldAABB, colliders[i]->aabb);
            }
            ScriptComponent::sp.ConstructNode(worldAABB, colliders);
            counter -= 0.02f;
        }

        auto childrenColor = UEngine::Color{ UEngine::Math::RndFloat(), UEngine::Math::RndFloat(), UEngine::Math::RndFloat(), 1 };
        ScriptComponent::sp.DebugRender(ScriptComponent::sp.head, colliders[0], Color{ 1, 0, 0, 1 }, Color{ 0,0,1,1 });
        for (auto collider : colliders)
            ScriptComponent::sp.DebugRender(ScriptComponent::sp.head, collider, Color{ 1, 1, 0, 1 });
    });

    for (size_t i = 0; i < colliders.size(); i++)
    {
        delete colliders[i];
    }

    return returnedValue;
}
