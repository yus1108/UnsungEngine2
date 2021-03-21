#include "stdafx.h"
#include "Game.h"

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool resize = false;
POINT size;

Game::Game(HINSTANCE hInstance, int width, int height)
{
    // Window Application
    app = WinApplication::Get();
    {
        WNDCLASSEXW Wcex;

        Wcex.cbSize = sizeof(WNDCLASSEX);

        Wcex.style = CS_HREDRAW | CS_VREDRAW;
        Wcex.lpfnWndProc = WndProc;
        Wcex.cbClsExtra = 0;
        Wcex.cbWndExtra = 0;
        Wcex.hInstance = hInstance;
        Wcex.hIcon = nullptr;
        Wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        Wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        Wcex.lpszMenuName = nullptr;
        Wcex.lpszClassName = L"UENGINE";
        Wcex.hIconSm = nullptr;

        UEngine::WINDOWS_APPLICATION_DESC desc;
        ZeroMemory(&desc, sizeof(desc));

        desc.HInstance = hInstance;
        desc.HasTitleBar = true;
        desc.InitUTime = true;
        desc.InitWinInput = true;
        desc.NCmdShow = 10;
        desc.TitleName = L"UEditor";
        desc.WindowSize = { width, height };
        desc.Wcex = &Wcex;

        app->Create(desc, 0);
    }

    screenSize = app->GetClientPixelSize();

    // DXRenderer
    renderer = UEngine::DXRenderer::Get();
    UEngine::DXRenderer::RENDERER_DESC rendererDesc = UEngine::DXRenderer::CreateDefaultInitDesc();
    {
        rendererDesc.IsDebuggable = true;
        rendererDesc.EnableAntialisedLine = true;
        rendererDesc.EnableBlendState = true;
        rendererDesc.EnableDepthStencil = false;
        rendererDesc.EnableMultisampling = true;
        rendererDesc.MultisampleDesc = { 4, 0 };
        rendererDesc.CullMode = D3D11_CULL_NONE;
        renderer->Init(app->GetHandler(), &rendererDesc);
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(app->GetHandler());
    ImGui_ImplDX11_Init(renderer->GetDevice(), renderer->GetImmediateDeviceContext());

    UEngine::SingletonManager::Init();
}

Game::~Game()
{
    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    UEngine::SingletonManager::Release();
}

void Game::Load()
{
    WinApplication::Get()->FreeDLL();
    WinApplication::Get()->LoadDLL(L"../Debug/DungeonSoulsScript.dll");

    typedef void(*ATTACH_SINGLETONS) (UEngine::SingletonManager::Singletons exportedSingletons);
    ATTACH_SINGLETONS attach_singletons = (ATTACH_SINGLETONS)WinApplication::Get()->FindFunction("ATTACH_SINGLETONS");
    attach_singletons(SingletonManager::Export());

    typedef UEngine::Component* (*AddScript) (UEngine::GameObject* gameObject);
    AddScript scriptCreation = NULL;
    
    // TODO: Place code here.
    {
        // basic load
        using namespace UEngine;
        auto size = app->GetClientPixelSize();
        GameScene* currentScene = new GameScene();
        currentScene->name = "tempScene";
        currentScene->Init(true);
        {
               auto cameraObject = GameObject::Instantiate(currentScene, "camera");
               auto camera = cameraObject->AddComponent<Camera>();
               camera->viewWidth.value = static_cast<float>(size.x);
               camera->viewHeight.value = static_cast<float>(size.y);
               currentScene->ResourceManager.ApplyChange();

               auto map = GameObject::Instantiate(currentScene, "map");
               map->AddComponent<RenderComponent>()->Load("rectangle", "image");
               map->AddComponent<Material>()->LoadImageMaterial(L"./Assets/background.png");
               map->GetTransform()->scale = Vector2(1280, 720);
               currentScene->ResourceManager.ApplyChange();

               for (size_t i = 0; i < 13; i++)
               {
                   int index = i;
                   auto tile = GameObject::Instantiate(currentScene, "tile");
                   tile->AddComponent<RenderComponent>()->Load("rectangle", "sprite");
                   tile->AddComponent<Material>()->LoadImageMaterial(L"./Assets/tileset.png");
                   tile->AddComponent<Physics2D::RectCollider>()->SetCollider(32, 32);
                   tile->GetTransform()->localPosition.value.x = (i + 1) * 32.0f;
                   tile->GetTransform()->scale.value = Vector2{
                       32,
                       32
                   };
                   tile->GetComponent<Material>()->uv = UV
                   {
                       7.0f / 12.0f,
                       0.5f,
                       8.0f / 12.0f,
                       1.0f - 1.0f / 3.0f
                   };
                   currentScene->ResourceManager.ApplyChange();
               }

               auto player = GameObject::Instantiate(currentScene, "player");
               player->AddComponent<RenderComponent>()->Load("rectangle", "sprite");
               player->AddComponent<UEngine::Physics2D::CircleCollider>()->SetCollider({ 0, 0 }, 16);

               auto playerImage = GameObject::Instantiate(currentScene, "playerImage");
               playerImage->AddComponent<RenderComponent>()->Load("rectangle", "sprite");
               auto material = playerImage->AddComponent<Material>();
               material->LoadImageMaterial(L"./Assets/Adventurer Sprite Sheet v1.3.png");
               material->uv.value = UV{ 0, 0, 1.0f / 13.0f, 1.0f / 13.0f };
               playerImage->GetTransform()->scale = Vector2(32, 32);
               playerImage->SetParent(player);

               scriptCreation = (AddScript)UEngine::WinApplication::Get()->FindFunction("PlayerCreation");
               scriptCreation(player);
               currentScene->ResourceManager.ApplyChange();

               

 /*              auto weapon = GameObject::Instantiate(currentScene, "weapon");
               weapon->AddComponent<RenderComponent>()->Load("rectangle", "image");
               weapon->AddComponent<Material>()->LoadImageMaterial(L"./Assets/throw_09.png");
               weapon->GetTransform()->scale = Vector2(32, 32);
               weapon->SetParent(player);

               scriptCreation = (AddScript)UEngine::WinApplication::Get()->FindFunction("WeaponCreation");
               scriptCreation(weapon);*/
        }
        //GameScene* currentScene = GameScene::LoadScene("./tempScene.uscene", true);

        

        GameState::Init(currentScene, true);
    }
}

int Game::Run(double targetHz)
{
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    auto returnedValue = app->UpdateLoop([&]()
    {
        if (targetHz > 0) UEngine::Utility::UTime::Get()->Throttle(targetHz);

        if (SingletonManager::State == nullptr)
        {
            SingletonManager::State = GameState::Get();
            Load();
            return;
        }
        if (GameState::IsTerminate())
        {
            GameState::Release();
            SingletonManager::State = nullptr;
            return;
        }
        if (resize)
        {
            UEngine::GameState::Get()->windowSize = UEngine::WinApplication::Get()->GetClientPixelSize();
            if (GameState::GetCurrentScene()->debugRenderer)
            {
                delete GameState::GetCurrentScene()->debugRenderer;
            }
            UEngine::DXRenderer::Get()->ResizeMainRenderTarget(size.x, size.y);
            if (GameState::GetCurrentScene()->debugRenderer)
            {
                GameState::GetCurrentScene()->debugRenderer = new DebugRenderer();
                GameState::GetCurrentScene()->debugRenderer->Init(DXRenderer::Get()->GetDevice(), DXRenderer::Get()->GetImmediateDeviceContext());
            }

            resize = false;
        }
        if (SingletonManager::State->noUpdate) return;
        GameState::Update([&]()
        {
            // Start the Dear ImGui frame
            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();
            //Console::Clear();
        }, [&]()
        {
            if (Input::GetKeyDown(VK_F10))
            {
                GameState::GetCurrentScene()->SaveScene();
                return true;
            }
            return false;
        }, [&]()
        {
            bool consoleFocused = false;
            Console::Render(&consoleFocused);
            SingletonManager::State->isFocused = !consoleFocused;
            // Rendering
            ImGui::Render();
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        });
    });

    return returnedValue;
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
    {
        if (UEngine::SingletonManager::App == nullptr) return 0;
        resize = true;
        size.x = (UINT)LOWORD(lParam);
        size.y = (UINT)HIWORD(lParam);
    }
    return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}