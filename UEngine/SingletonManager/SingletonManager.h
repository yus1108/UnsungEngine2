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
		};
		static WinApplication* App;
		static WinInput* Input;
		static DXRenderer::DXRenderer* Renderer;
		static Utility::UTime* Time;

		static void Init();
		static void Release();

		static Singletons Export();
		static void Import(Singletons singletons);
	};
}


