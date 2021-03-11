#include "DXRFramework.h"
#include "DXRenderMesh.h"

namespace UEngine
{
	namespace DXRenderer
	{
		DXRenderMesh* const DXRenderMesh::Instantiate
		(
			ID3D11Device* const device,
			const std::vector<SIMPLE_VERTEX>& vertices,
			D3D11_PRIMITIVE_TOPOLOGY topology
		)
		{
			DXRenderMesh* instance = new DXRenderMesh;
			instance->SetVertices<SIMPLE_VERTEX>(device, &vertices[0], vertices.size());
			instance->vertices = vertices;
			instance->SetTopology(topology);
			return instance;
		}

		DXRenderMesh* const DXRenderMesh::Instantiate
		(
			ID3D11Device* const device,
			const std::vector<SIMPLE_VERTEX>& vertices,
			const std::vector<unsigned>& indices,
			D3D11_PRIMITIVE_TOPOLOGY topology
		)
		{
			DXRenderMesh* instance = new DXRenderMesh;
			instance->SetVertices<SIMPLE_VERTEX>(device, &vertices[0], vertices.size());
			instance->vertices = vertices;
			instance->SetIndices(device, &indices[0], indices.size());
			instance->indices = indices;
			instance->SetTopology(topology);
			return instance;
		}

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

		void DXRenderMesh::Release(DXRenderMesh* const renderMesh)
		{
			delete renderMesh;
		}

		void DXRenderMesh::Set(ID3D11DeviceContext* const deviceContext)
		{
			UINT offset = 0;

			deviceContext->IASetPrimitiveTopology(topology);
			deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
			deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		}

		void DXRenderMesh::Draw(ID3D11DeviceContext* const deviceContext)
		{
			if (indexCount == 0)
			{
				deviceContext->Draw(vertexCount, 0);
				return;
			}
			deviceContext->DrawIndexed(indexCount, 0, 0);
		}

	}
}