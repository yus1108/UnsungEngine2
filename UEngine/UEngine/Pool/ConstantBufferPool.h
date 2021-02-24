#pragma once

namespace UEngine
{
	class ConstantBufferPool
	{
	private:
		std::queue<DXRenderer::DXConstantBuffer*> creationQueue;
		std::map<DXRenderer::DXConstantBuffer*, DXRenderer::DXConstantBuffer*> pool;
		std::map<DXRenderer::DXConstantBuffer*, DXRenderer::DXConstantBuffer*> deletionMap;

	public:
		~ConstantBufferPool();
		void Add(DXRenderer::DXConstantBuffer* const constantBuffer) { creationQueue.push(constantBuffer); }
		void Remove(DXRenderer::DXConstantBuffer* const constantBuffer) { deletionMap[constantBuffer] = constantBuffer; }
		void OnPreRender();
	};
}
