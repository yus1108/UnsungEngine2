#pragma once

namespace UEngine
{
	class SingletonManager
	{
	public:
		struct Singletons
		{
			WinApplication* App = nullptr;
			WinInput* Input = nullptr;
			DXRenderer::DXRenderer* Renderer = nullptr;
			Utility::UTime* Time = nullptr;
			GameState* State;
			Console* _Console;
		};
		static WinApplication* App;
		static WinInput* Input;
		static DXRenderer::DXRenderer* Renderer;
		static Utility::UTime* Time;
		static GameState* State;
		static Console* _Console;

		static void Init();
		static void Release();

		static Singletons Export();
		static void Import(Singletons singletons);
	};
}


