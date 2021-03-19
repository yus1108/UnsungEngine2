#pragma once

namespace UEngine
{
	namespace UEditor
	{
		class EditorState
		{
		private:
			POINT screenSize;
			WinApplication* app = nullptr;
			DXRenderer::DXRenderer* renderer = nullptr;
		public:
			EditorState() = delete;
			EditorState(HINSTANCE hInstance, int width = 800, int height = 600);
			~EditorState();
			void Load();
			int Run(double targetHz = -1);
		};
	}
}