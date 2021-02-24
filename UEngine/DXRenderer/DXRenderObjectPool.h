#pragma once

namespace UEngine
{
	namespace DXRenderer
	{
		class DXRenderObjectPool
		{
#pragma region Singleton
		public:
			static DXRenderObjectPool* Get() { return &instance; }

		private:
			DXRenderObjectPool() = default;
			~DXRenderObjectPool();
			static DXRenderObjectPool instance;
#pragma endregion

		private:
			std::vector<DXRenderObject*> pool;

		public:
			DXRenderObject* LoadObject(std::string shader, std::string renderMesh, std::list<CONSTANT_BUFFER_DESC> bufferDescs);
			const std::vector<DXRenderObject*>* const GetList() { return &pool; }
			void Remove(DXRenderObject* renderObject);
		};
	}
}


