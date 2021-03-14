#include "../UEngine/UEngine.h"
#include "SingletonManager.h"

namespace UEngine
{
	WinApplication* SingletonManager::App = nullptr;
	WinInput* SingletonManager::Input = nullptr;
	DXRenderer::DXRenderer* SingletonManager::Renderer = nullptr;
	Utility::UTime* SingletonManager::Time = nullptr;
}
void UEngine::SingletonManager::Init()
{
	App = WinApplication::Get();
	Input = WinInput::Get();
	Renderer = DXRenderer::Get();
	Time = Utility::UTime::Get();
}

void UEngine::SingletonManager::Release()
{
	App->Release();
	Input->Release();
	Renderer->Release();
	Time->Release();
}

UEngine::SingletonManager::Singletons UEngine::SingletonManager::Export()
{
	Singletons singletons
	{
		App,
		Input,
		Renderer,
		Time
	};
	return singletons;
}

void UEngine::SingletonManager::Import(Singletons singletons)
{
	App = singletons.App;
	Input = singletons.Input;
	Renderer = singletons.Renderer;
	Time = singletons.Time;

	WinApplication::Attach(App);
	WinInput::Attach(Input);
	DXRenderer::DXRenderer::Attach(Renderer);
	Utility::UTime::Attach(Time);
}
