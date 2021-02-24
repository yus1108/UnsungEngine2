#pragma once

namespace UEngine
{
	namespace DXRenderer
	{
		class DXRenderObjectPool
		{
		private:
			std::vector<DXRenderObject*> pool;
			std::queue<DXRenderObject*> creationQueue;
			std::map<DXRenderObject*, DXRenderObject*> deletionMap;

		public:
			~DXRenderObjectPool();
			DXRenderObject* LoadObject(std::string shader, std::string renderMesh);
			void Remove(DXRenderObject* renderObject) { deletionMap[renderObject] = renderObject; }
			void Update();
		};
	}
}


