#include "DXRFramework.h"
#include "DXRenderMesh.h"

namespace UEngine
{
	void DXRenderMesh::SetIndices(ID3D11Device* const device, const unsigned* const indices, UINT indexCount)
	{
		this->indexCount = indexCount;
		D3D11_BUFFER_DESC bufferDesc;
		// Fill in a buffer description.
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		bufferDesc.ByteWidth = (UINT)(sizeof(unsigned int) * indexCount);
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		// Define the resource data.
		D3D11_SUBRESOURCE_DATA initData;
		ZeroMemory(&initData, sizeof(initData));
		initData.pSysMem = indices;
		device->CreateBuffer(&bufferDesc, &initData, indexBuffer.GetAddressOf());
	}

	void DXRenderMesh::Release(DXRenderMesh** const renderMesh)
	{
		delete* renderMesh;
		*renderMesh = nullptr;
	}

	void DXRenderMesh::Set(ID3D11DeviceContext* const deviceContext)
	{
		UINT offset = 0;

		deviceContext->IASetPrimitiveTopology(topology);
		deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
		deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}
}