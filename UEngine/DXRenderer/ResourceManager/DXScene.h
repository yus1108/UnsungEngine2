#pragma once

namespace UEngine
{
	namespace DXRenderer
	{
		class DXScene final
		{
			friend class DXResourceManager;
		private:
			DXScene() = default;
			~DXScene() = default;

		public:
			std::unordered_map<std::string, const struct SHARED_RENDERMESH*> loadedRenderMeshes;
		};
	}
}

