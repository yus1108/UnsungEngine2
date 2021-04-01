#include "UEditor.h"
#include "EditorState.h"

#define IMGUI_BORDER_PADDING 8
#define IMGUI_TITLEBAR_PADDING_Y 27
#define IMGUI_RIGHT_PADDING_X 4

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool resize = false;
POINT window_size;

UEngine::UEditor::EditorState::EditorState(HINSTANCE hInstance, int width, int height)
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

    audio.InitAudio();
    audio.PlayAudio(0, L"Assets/BRPG_Assault_noGT_Loop.wav", AudioType::AudioType_Music);
}

UEngine::UEditor::EditorState::~EditorState()
{
    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    UEngine::SingletonManager::Release();
    audio.StopAudio();
}

void UEngine::UEditor::EditorState::Load()
{
    WinApplication::Get()->FreeDLL();
    WinApplication::Get()->LoadDLL(L"./DllTest.dll");

    typedef void(*ATTACH_SINGLETONS) (UEngine::SingletonManager::Singletons exportedSingletons);
    ATTACH_SINGLETONS attach_singletons = (ATTACH_SINGLETONS)WinApplication::Get()->FindFunction("ATTACH_SINGLETONS");
    attach_singletons(SingletonManager::Export());

    // TODO: Place code here.
    {
        // basic load
        //using namespace UEngine;
        //GameScene* currentScene = new GameScene();
        //currentScene->name = "tempScene";
        //currentScene->Init(true);
        //{
        //    auto editorObj = GameObject::Instantiate(currentScene, "editorCamera");
        //    auto editorCamera = editorObj->AddComponent<Camera>();
        //    editorCamera->viewWidth.value = 15;
        //    editorCamera->viewHeight.value = 30;
        //    currentScene->EditorCamera = editorCamera;

        //    auto cameraObject = GameObject::Instantiate(currentScene, "camera");
        //    auto camera = cameraObject->AddComponent<Camera>();
        //    camera->SetMainCamera();
        //    camera->viewWidth.value = 15;
        //    camera->viewHeight.value = 30;

        //    auto ball = GameObject::Instantiate(currentScene, "ball");
        //    ball->AddComponent<RenderComponent>()->Load("rectangle", "image");
        //    ball->AddComponent<Material>()->LoadImageMaterial(L"./Assets/soccer_ball.png");
        //}
        GameScene* currentScene = GameScene::LoadScene("./tempScene.uscene", true);


        GameState::Init(currentScene, false);
    }
}

int UEngine::UEditor::EditorState::Run(double targetHz)
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
            UEngine::DXRenderer::Get()->ResizeMainRenderTarget(window_size.x, window_size.y);
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
            ImGui::ShowDemoWindow(&show_demo_window);

            // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
            {
                static float f = 0.0f;
                static int counter = 0;
                
                ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

                ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
                ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
                ImGui::Checkbox("Another Window", &show_another_window);

                ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

                if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                    counter++;
                ImGui::SameLine();
                ImGui::Text("counter = %d", counter);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }

            // 3. Show another simple window.
            if (show_another_window)
            {
                ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
                ImGui::Text("Hello from another window!");
                if (ImGui::Button("Close Me"))
                    show_another_window = false;
                ImGui::End();
            }

            // Scene Editor
            {
                ImGui::SetNextWindowBgAlpha(1.0f);
                ImGui::Begin((std::string("Scene: ") + GameState::GetCurrentScene()->name).c_str());
                GameState::Get()->isFocused = ImGui::IsWindowFocused();
                ImGui::BeginGroup(); // Lock X position
                ImVec2 pos = ImGui::GetWindowPos();
                SingletonManager::State->startWindowPos.x = pos.x + IMGUI_BORDER_PADDING;
                SingletonManager::State->startWindowPos.y = pos.y + IMGUI_TITLEBAR_PADDING_Y;

                ImVec2 size = ImGui::GetWindowSize();
                if (size.x < 200 & size.y < 200)
                {
                    size.x = 200;
                    size.y = 200;
                    ImGui::SetWindowSize(size);
                }
                size.x = size.x - IMGUI_BORDER_PADDING - IMGUI_BORDER_PADDING;
                size.y = size.y - IMGUI_TITLEBAR_PADDING_Y - IMGUI_BORDER_PADDING * 2.0f;
                SingletonManager::State->windowSize.x = size.x;
                SingletonManager::State->windowSize.y = size.y;

                ImGui::InvisibleButton("##empty", size);
                const ImVec2 p0 = ImGui::GetItemRectMin();
                const ImVec2 p1 = ImGui::GetItemRectMax();
                SingletonManager::State->windowSize.x = p1.x - p0.x;
                SingletonManager::State->windowSize.y = p1.y - p0.y;
                ImDrawList* draw_list = ImGui::GetWindowDrawList();
                draw_list->AddRectFilled(p0, p1, IM_COL32(0, 0, 0, 255));
                draw_list->AddImage(GameState::GetCurrentScene()->EditorCamera->GetView()->view->GetViewResource(), p0, p1);
                ImGui::EndGroup();
                ImGui::End();
            }

            SingletonManager::State->ShowHierarchy();
            Console::Render();

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
        window_size.x = (UINT)LOWORD(lParam);
        window_size.y = (UINT)HIWORD(lParam);
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