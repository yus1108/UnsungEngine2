#pragma once

class GameObject;

class RenderObject
{
	UEngine::DXRenderer::DXShader* shader;
	UEngine::DXRenderer::DXRenderMesh* renderMesh;
	std::map<std::string, UEngine::DXRenderer::DXConstantBuffer*> constantBuffers;
	std::map<std::wstring, UEngine::DXRenderer::DXTexture*> textures;
};

class GameView
{
	UEngine::DXRenderer::DXView* view;
	std::vector<RenderObject*> renderObjects;
};

class GameScene
{
	std::list<GameObject*> gameObjects;
	std::vector<GameView*> cpu_view;
	std::vector<GameView*> gpu_view;
	UEngine::DXRenderer::DXResourceManager ResourceManager;

	template <typename T>
	T* LoadResource(std::wstring resource_name);

};

class GameState
{
private:
	std::map<std::wstring, GameScene*> scenes;
	GameScene* runningScene = nullptr;
};

template<typename T>
inline T* GameScene::LoadResource(std::wstring resource_name)
{
	auto resource = ResourceManager->GetResource<T>(resource_name);
	if (resource == nullptr)
	{
		// TODO: open file and load resource using reousrce_name as file name
	}
	return resource;
	return nullptr;
}
