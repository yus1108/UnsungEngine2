#pragma once

namespace DXRenderer
{
	class DXScene final
	{
	public:
		std::unordered_map<std::string, const struct SHARED_RENDERMESH*> loadedRenderMeshes;
	};
}
