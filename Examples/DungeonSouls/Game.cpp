#include "stdafx.h"
#include "Game.h"

#define SCENELOAD 0

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

        app->Create(desc, 4);
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
    WinApplication::Get()->LoadDLL(L"./DungeonSoulsScript.dll");

    typedef void(*ATTACH_SINGLETONS) (UEngine::SingletonManager::Singletons exportedSingletons);
    ATTACH_SINGLETONS attach_singletons = (ATTACH_SINGLETONS)WinApplication::Get()->FindFunction("ATTACH_SINGLETONS");
    attach_singletons(SingletonManager::Export());

    typedef UEngine::Component* (*AddScript) (UEngine::GameObject* gameObject);
    AddScript scriptCreation = NULL;
    
    // TODO: Place code here.
    {
#if SCENELOAD
        // basic load
        using namespace UEngine;
        auto size = app->GetClientPixelSize();
        GameScene* currentScene = new GameScene();
        currentScene->name = "tempScene";
        currentScene->Init(true);
        {
               auto cameraObject = GameObject::Instantiate(currentScene, "worldCamera");
               auto camera = cameraObject->AddComponent<Camera>();
               camera->viewWidth.value = static_cast<float>(size.x);
               camera->viewHeight.value = static_cast<float>(size.y);
               currentScene->ResourceManager.ApplyChange();

               auto playerCameraObj = GameObject::Instantiate(currentScene, "followCamera");
               auto playerCamera = playerCameraObj->AddComponent<Camera>();
               playerCamera->viewWidth.value = 640.0f;
               playerCamera->viewHeight.value = 376.0f;
               playerCamera->SetMainCamera();
               currentScene->ResourceManager.ApplyChange();

               auto map = GameObject::Instantiate(currentScene, "map");
               map->AddComponent<RenderComponent>()->Load("rectangle", "image");
               map->AddComponent<Material>()->LoadImageMaterial(L"./Assets/background.png");
               map->GetTransform()->scale = Vector2(1280, 720);
               currentScene->ResourceManager.ApplyChange();

               for (size_t i = 0; i < 30; i++)
               {
                   int index = i - 15;
                   auto tile = GameObject::Instantiate(currentScene, "tile");
                   tile->IsStatic = true;
                   tile->AddComponent<RenderComponent>()->Load("rectangle", "sprite");
                   tile->AddComponent<Material>()->LoadImageMaterial(L"./Assets/tileset.png");
                   tile->AddComponent<Physics2D::RectCollider>()->SetCollider(32, 32);
                   tile->GetTransform()->localPosition.value.x = index * 32.0f;
                   tile->GetTransform()->localPosition.value.y = -32.0f;
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

               for (size_t i = 0; i < 1; i++)
               {
                   auto enemy = GameObject::Instantiate(currentScene, "enemy");
                   auto enemyCollider = enemy->AddComponent<UEngine::Physics2D::CircleCollider>();
                   enemyCollider->SetCollider({ 0, 0 }, 16);
                   enemyCollider->IsTrigger = true;
                   enemy->GetTransform()->localPosition.value.x = 50.0f;
                   enemy->GetTransform()->localPosition.value.y = 200.0f;
                   scriptCreation = (AddScript)UEngine::WinApplication::Get()->FindFunction("SkeletonCreation");
                   scriptCreation(enemy);
                   currentScene->ResourceManager.ApplyChange();

                   auto enemyBody = GameObject::Instantiate(currentScene, "enemyBody");
                   enemyBody->AddComponent<RenderComponent>()->Load("rectangle", "sprite");
                   auto enemyBodyCollider = enemyBody->AddComponent<UEngine::Physics2D::RectCollider>();
                   enemyBodyCollider->SetCollider(16, 32);
                   enemyBodyCollider->IsTrigger = true;
                   auto enemyMaterial = enemyBody->AddComponent<Material>();
                   enemyMaterial->LoadImageMaterial(L"./Assets/SkeletalWarrior_Sprites.png");
                   enemyMaterial->uv.value = UV{ 0, 0, 1.0f / 10.0f, 1.0f / 10.0f };
                   enemyBody->GetTransform()->scale = Vector2(32, 32);
                   enemyBody->SetParent(enemy);
                   scriptCreation = (AddScript)UEngine::WinApplication::Get()->FindFunction("HealthCreation");
                   scriptCreation(enemyBody);
                   currentScene->ResourceManager.ApplyChange();

                   auto enemyAttack = GameObject::Instantiate(currentScene, "enemyAttack");
                   auto eAttack = enemyAttack->AddComponent<UEngine::Physics2D::RectCollider>();
                   eAttack->SetCollider(16, 32);
                   eAttack->IsTrigger = true;
                   enemyAttack->GetTransform()->localPosition.value.x = 8;
                   scriptCreation = (AddScript)UEngine::WinApplication::Get()->FindFunction("WeaponCreation");
                   scriptCreation(enemyAttack);
                   enemyAttack->SetParent(enemy);
                   currentScene->ResourceManager.ApplyChange();
               }

               auto player = GameObject::Instantiate(currentScene, "player");
               player->AddComponent<RenderComponent>()->Load("rectangle", "sprite");
               player->AddComponent<UEngine::Physics2D::CircleCollider>()->SetCollider({ 0, 0 }, 16);
               player->GetTransform()->localPosition.value.y = 200.0f;
               scriptCreation = (AddScript)UEngine::WinApplication::Get()->FindFunction("FollowCameraCreation");
               scriptCreation(player);
               scriptCreation = (AddScript)UEngine::WinApplication::Get()->FindFunction("PlayerCreation");
               scriptCreation(player);
               currentScene->ResourceManager.ApplyChange();

               auto playerBody = GameObject::Instantiate(currentScene, "playerBody");
               playerBody->AddComponent<RenderComponent>()->Load("rectangle", "sprite");
               auto material = playerBody->AddComponent<Material>();
               material->LoadImageMaterial(L"./Assets/Adventurer Sprite Sheet v1.3.png");
               material->uv.value = UV{ 0, 0, 1.0f / 13.0f, 1.0f / 13.0f };
               playerBody->GetTransform()->scale = Vector2(32, 32);
               playerBody->SetParent(player);
               currentScene->ResourceManager.ApplyChange();

               auto playerHealth = GameObject::Instantiate(currentScene, "playerHealth");
               auto playerHealthCollider = playerHealth->AddComponent<UEngine::Physics2D::RectCollider>();
               playerHealthCollider->SetCollider(16, 32);
               playerHealthCollider->IsTrigger = true;
               scriptCreation = (AddScript)UEngine::WinApplication::Get()->FindFunction("HealthCreation");
               scriptCreation(playerHealth);
               playerHealth->SetParent(player);
               currentScene->ResourceManager.ApplyChange();

               auto attackCollider = GameObject::Instantiate(currentScene, "attackCollider");
               auto attack = attackCollider->AddComponent<UEngine::Physics2D::RectCollider>();
               attack->SetCollider(16, 32);
               attack->IsTrigger = true;
               attackCollider->GetTransform()->localPosition.value.x = 8;
               attackCollider->SetParent(player);
               scriptCreation = (AddScript)UEngine::WinApplication::Get()->FindFunction("WeaponCreation");
               scriptCreation(attackCollider);
               currentScene->ResourceManager.ApplyChange();
        }
#else
        GameScene* currentScene = GameScene::LoadScene("./tempScene.uscene", false);
        currentScene->InitDebugMode(true);
#endif
        

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
            Console::Sync();
        }, nullptr, [&]()
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