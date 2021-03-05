#pragma once

namespace UEngine
{
	namespace DXRenderer
	{
		class DXScene
		{
		private:
			std::wstring name;

		public:
			DXScene() = delete;
			DXScene(const DXScene&) = delete;
			DXScene(std::wstring name) : name(name) {}
			~DXScene() = default;

			const std::wstring GetName() { return name; }

			void Begin();
			void Render();
			void End();
		};
	}
}