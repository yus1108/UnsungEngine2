#include "../UEngine/UEngine.h"
#include "SingletonManager.h"

namespace UEngine
{
	WinApplication* SingletonManager::App = nullptr;
	WinInput* SingletonManager::Input = nullptr;
	DXRenderer::DXRenderer* SingletonManager::Renderer = nullptr;
	Utility::UTime* SingletonManager::Time = nullptr;
	GameState* SingletonManager::State = nullptr;
	Console* SingletonManager::_Console = nullptr;
}
void UEngine::SingletonManager::Init()
{
	App = WinApplication::Get();
	Input = WinInput::Get();
	Time = Utility::UTime::Get();
	Renderer = DXRenderer::Get();
	State = GameState::Get();
	_Console = Console::Get();
}

void UEngine::SingletonManager::Release()
{
	State->Release();
	_Console->Release();
	Renderer->Release();
	Input->Release();
	Time->Release();
	App->Release();
}

UEngine::SingletonManager::Singletons UEngine::SingletonManager::Export()
{
	Singletons singletons
	{
		App,
		Input,
		Renderer,
		Time,
		State,
		_Console
	};
	return singletons;
}

void UEngine::SingletonManager::Import(Singletons singletons)
{
	App = singletons.App;
	Input = singletons.Input;
	Renderer = singletons.Renderer;
	Time = singletons.Time;
	State = singletons.State;
	_Console = singletons._Console;

	WinApplication::Attach(App);
	WinInput::Attach(Input);
	DXRenderer::DXRenderer::Attach(Renderer);
	Utility::UTime::Attach(Time);
	GameState::Attach(State);
	Console::Attach(_Console);
}
