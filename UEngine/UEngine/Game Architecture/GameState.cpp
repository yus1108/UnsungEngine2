#include "UEngine.h"
#include "GameState.h"

UEngine::GameState* UEngine::GameState::instance = nullptr;

bool UEngine::GameState::IsFixedUpdate()
{
	float timestep = GetCurrentFixedTimestep();
	instance->fixedUpdateTimer -= timestep;
	return instance->fixedUpdateTimer < timestep;
}

void UEngine::GameState::SetCurrentScene(std::string name)
{
	auto scene = GetScene(name);
	if (!scene) throw std::runtime_error("Cannot set nullptr as the current scene of the game state");
	instance->currentScene = scene;
}

UEngine::GameScene* UEngine::GameState::GetScene(std::string name)
{
	auto iter = instance->scenes.find(name);
	if (iter == instance->scenes.end())
		return nullptr;
	return iter->second;
}

void UEngine::GameState::Init(bool drawOnBackBuffer)
{
	if (instance == nullptr) instance = new GameState;
	instance->isTerminate = false;
	instance->drawOnBackBuffer = drawOnBackBuffer;
	instance->windowSize = WinApplication::Get()->GetClientPixelSize();
}

void UEngine::GameState::Init(GameScene* scene, bool drawOnBackBuffer)
{
	if (instance == nullptr) instance = new GameState;
	instance->isTerminate = false;
	instance->drawOnBackBuffer = drawOnBackBuffer;
	instance->currentScene = scene;
	instance->scenes[scene->name] = scene;
	instance->windowSize = WinApplication::Get()->GetClientPixelSize();
}

void UEngine::GameState::AddScene(GameScene* scene, bool setCurrentScene)
{
	instance->scenes[scene->name] = scene;
	if (setCurrentScene) instance->currentScene = scene;
}

void UEngine::GameState::Update(std::function<void()> OnSync, std::function<bool()> OnUpdate, std::function<void()> OnRender)
{
	std::lock_guard<std::mutex> lock(instance->noRenderMutex);
	instance->deltaTime = Utility::UTime::Get()->DeltaTimeF();
	instance->fixedUpdateTimer += instance->deltaTime;

	if (OnSync) OnSync();
	instance->currentScene->Sync();

	WinApplication::Get()->threadPool.AddSyncTask([&]()
	{
		instance->currentScene->Render(DXRenderer::Get()->GetImmediateDeviceContext());
		
		DXRenderer::Get()->Begin(DXRenderer::Get()->GetContext());

		if (instance->drawOnBackBuffer)
		{
			DXRenderer::Get()->Draw(DXRenderer::Get()->GetContext(), instance->currentScene->MainView->view->GetAddressOfViewResource());
			if (instance->currentScene->IsDebugMode() && instance->currentScene->debugRenderer)
				DXRenderer::Get()->Draw(DXRenderer::Get()->GetContext(), instance->currentScene->debugRenderer->GetAddressOfViewResource());
		}

		if (OnRender) OnRender();
		DXRenderer::Get()->End(nullptr);
	});

	if (OnUpdate) instance->isTerminate = OnUpdate();
	
	instance->currentScene->Update();
	WinApplication::Get()->threadPool.Join();


	instance->currentScene->PostRender();
}

void UEngine::GameState::RenderHierarchy(UEngine::GameObject* gameObject)
{
    ImGuiIO& io = ImGui::GetIO();

    for (auto child : gameObject->GetChildren())
    {
        ImGui::PushID(&child->name);
        if (ImGui::TreeNode((
            child->name + " : GameObject(" +
            std::to_string(child->GetChildren().size()) + ")").c_str()))
        {
            for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
            {
                if (ImGui::IsMouseClicked(i) && i == 1 && ImGui::IsItemHovered())
                    ImGui::OpenPopup("gameobject_selection_popup2");
            }
            if (ImGui::BeginPopup("gameobject_selection_popup2"))
            {
                ImGui::MenuItem("copy", "Ctrl+C", false);
                ImGui::MenuItem("paste", "Ctrl+V", false, false);
                ImGui::Separator();
                ImGui::MenuItem("rename", "F2", false, false);
                ImGui::MenuItem("duplicate", "Ctrl+G", false);
                ImGui::MenuItem("delete", "del", false);
                ImGui::Separator();
                if (ImGui::MenuItem("Create Empty", "", false))
                {
                    auto newChild = GameObject::Instantiate(child->GetScene(), "GameObject");
                    newChild->AddComponent<EditorScript>();
                    newChild->SetParent(child);
                }
                ImGui::MenuItem("2D Object", "", false);
                if (ImGui::MenuItem("Camera", "", false))
                {
                    auto newChild = GameObject::Instantiate(child->GetScene(), "GameObject");
                    newChild->AddComponent<EditorScript>();
                    newChild->AddComponent<Camera>();
                    newChild->SetParent(child);
                }
                ImGui::EndPopup();
            }
            if (ImGui::Button("Select"))
            {
                UEngine::EditorScript::isSelected = child->GetComponent<UEngine::EditorScript>();
            }
            RenderHierarchy(child);
            ImGui::TreePop();
        }
        else
        {
            for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
            {
                if (ImGui::IsMouseClicked(i) && i == 1 && ImGui::IsItemHovered())
                    ImGui::OpenPopup("gameobject_selection_popup");
            }
            if (ImGui::BeginPopup("gameobject_selection_popup"))
            {
                ImGui::MenuItem("copy", "Ctrl+C", false);
                ImGui::MenuItem("paste", "Ctrl+V", false, false);
                ImGui::Separator();
                ImGui::MenuItem("rename", "F2", false, false);
                ImGui::MenuItem("duplicate", "Ctrl+G", false);
                ImGui::MenuItem("delete", "del", false);
                ImGui::Separator();
                if (ImGui::MenuItem("Create Empty", "", false))
                {
                    auto newChild = GameObject::Instantiate(child->GetScene(), "GameObject");
                    newChild->AddComponent<EditorScript>();
                    newChild->SetParent(child);
                }
                ImGui::MenuItem("2D Object", "", false);
                if (ImGui::MenuItem("Camera", "", false))
                {
                    auto newChild = GameObject::Instantiate(child->GetScene(), "GameObject");
                    newChild->AddComponent<EditorScript>();
                    newChild->AddComponent<Camera>();
                    newChild->SetParent(child);
                }
                ImGui::EndPopup();
            }
        }
        ImGui::PopID();
    }
}

void UEngine::GameState::ShowHierarchy()
{
    ImGui::Begin("GameState Hierarchy");

    static GameScene* sceneSelected = nullptr;
    ImGuiIO& io = ImGui::GetIO();

    for (auto sceneMap : instance->GetScenes())
    {
        ImGui::PushID(&sceneMap.first);
        if (ImGui::CollapsingHeader((sceneMap.first + " : GameScene").c_str()))
        {
            sceneSelected = sceneMap.second;
            for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
            {
                if (ImGui::IsMouseClicked(i) && i == 1 && ImGui::IsItemHovered())
                    ImGui::OpenPopup("scene_selection_popup");
            }

            for (auto gameObject : sceneMap.second->GetGameObjects())
            {
                if (gameObject->GetParent() == nullptr)
                {
                    ImGui::PushID(&gameObject->name);
                    static bool closable_group = true;
                    if (ImGui::TreeNode((gameObject->name + " : GameObject(" +
                        std::to_string(gameObject->GetChildren().size()) + ")").c_str()))
                    {
                        for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
                        {
                            if (ImGui::IsMouseClicked(i) && i == 1 && ImGui::IsItemHovered())
                                ImGui::OpenPopup("gameobject_selection_popup2");
                        }
                        if (ImGui::BeginPopup("gameobject_selection_popup2"))
                        {
                            ImGui::MenuItem("copy", "Ctrl+C", false);
                            ImGui::MenuItem("paste", "Ctrl+V", false, false);
                            ImGui::Separator();
                            ImGui::MenuItem("rename", "F2", false, false);
                            ImGui::MenuItem("duplicate", "Ctrl+G", false);
                            ImGui::MenuItem("delete", "del", false);
                            ImGui::Separator();
                            if (ImGui::MenuItem("Create Empty", "", false))
                            {
                                auto child = GameObject::Instantiate(sceneSelected, "GameObject");
                                child->AddComponent<EditorScript>();
                                child->SetParent(gameObject);
                            }
                            ImGui::MenuItem("2D Object", "", false);
                            if (ImGui::MenuItem("Camera", "", false))
                            {
                                auto child = GameObject::Instantiate(sceneSelected, "GameObject");
                                child->AddComponent<EditorScript>();
                                child->AddComponent<Camera>();
                                child->SetParent(gameObject);
                            }
                            ImGui::EndPopup();
                        }
                        if (ImGui::Button("Select"))
                        {
                            UEngine::EditorScript::isSelected = gameObject->GetComponent<EditorScript>();
                        }
                        instance->RenderHierarchy(gameObject);
                        ImGui::TreePop();
                    }
                    else
                    {
                        for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
                        {
                            if (ImGui::IsMouseClicked(i) && i == 1 && ImGui::IsItemHovered())
                                ImGui::OpenPopup("gameobject_selection_popup");
                        }
                    }
                    if (ImGui::BeginPopup("gameobject_selection_popup"))
                    {
                        ImGui::MenuItem("copy", "Ctrl+C", false);
                        ImGui::MenuItem("paste", "Ctrl+V", false, false);
                        ImGui::Separator();
                        ImGui::MenuItem("rename", "F2", false, false);
                        ImGui::MenuItem("duplicate", "Ctrl+G", false);
                        ImGui::MenuItem("delete", "del", false);
                        ImGui::Separator();
                        if (ImGui::MenuItem("Create Empty", "", false))
                        {
                            auto child = GameObject::Instantiate(sceneSelected, "GameObject");
                            child->AddComponent<EditorScript>();
                            child->SetParent(gameObject);
                        }
                        ImGui::MenuItem("2D Object", "", false);
                        if (ImGui::MenuItem("Camera", "", false))
                        {
                            auto child = GameObject::Instantiate(sceneSelected, "GameObject");
                            child->AddComponent<EditorScript>();
                            child->AddComponent<Camera>();
                            child->SetParent(gameObject);
                        }
                        ImGui::EndPopup();
                    }
                    ImGui::PopID();
                }
            }
        }
        else
        {
            for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
            {
                if (ImGui::IsMouseClicked(i) && i == 1 && ImGui::IsItemHovered())
                    ImGui::OpenPopup("scene_selection_popup");
            }
        }

        if (ImGui::BeginPopup("scene_selection_popup"))
        {
            ImGui::MenuItem("copy", "Ctrl+C", false);
            ImGui::MenuItem("paste", "Ctrl+V", false, false);
            ImGui::Separator();
            ImGui::MenuItem("rename", "F2", false, false);
            ImGui::MenuItem("duplicate", "Ctrl+G", false);
            ImGui::MenuItem("delete", "del", false);
            ImGui::Separator();
            if (ImGui::MenuItem("Create Empty", "", false))
            {
                auto gameObject = GameObject::Instantiate(sceneSelected, "GameObject");
                gameObject->AddComponent<EditorScript>();
            }
            ImGui::MenuItem("2D Object", "", false);
            if (ImGui::MenuItem("Camera", "", false))
            {
                auto gameObject = GameObject::Instantiate(sceneSelected, "GameObject");
                gameObject->AddComponent<EditorScript>();
                gameObject->AddComponent<Camera>();
            }
            ImGui::EndPopup();
        }
        ImGui::PopID();
    }

    ImGui::End();
}

void UEngine::GameState::Release()
{
	if (instance == nullptr) return;
	instance->isTerminate = true;
	for (auto scene : instance->scenes)
		delete scene.second;
	instance->scenes.clear();
	instance->currentScene = nullptr;
	delete instance;
	instance = nullptr;
}
