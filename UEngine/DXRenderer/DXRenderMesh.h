#pragma once
#include <vector>
#include "dxrframework.h"

namespace UEngine
{
	template <typename STRUCT_VERTEX>
	class RenderMesh final
	{
	private:
		RenderMesh() {}
		~RenderMesh() = default;
	private:
		ID3D11Device* device{ nullptr };
		ID3D11DeviceContext* deviceContext{ nullptr };

		std::vector<STRUCT_VERTEX> vertices;
		std::vector<unsigned> indices;	
		D3D11_PRIMITIVE_TOPOLOGY topology{ D3D_PRIMITIVE_TOPOLOGY_UNDEFINED };

		Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	public:
		void SetVertices(const std::vector<STRUCT_VERTEX>& vertices);
		void SetIndices(const std::vector<unsigned>& indices);
		void SetTopology(D3D11_PRIMITIVE_TOPOLOGY topology) { this->topology = topology; }

		const std::vector<STRUCT_VERTEX>& GetVertices() const { return vertices; }
		unsigned GetVerticesCount() { return vertices.size(); }
		unsigned GetIndicesCount() { return indices.size(); }

		static RenderMesh<STRUCT_VERTEX>* const Init(ID3D11Device* const device, ID3D11DeviceContext* const deviceContext);
		static void Release(RenderMesh<STRUCT_VERTEX>** const renderMesh);
		void Render();
	};	


	template <typename STRUCT_VERTEX>
	void RenderMesh<STRUCT_VERTEX>::SetVertices(const std::vector<STRUCT_VERTEX>& vertices)
	{
		this->vertices = vertices;

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.ByteWidth = (UINT)(sizeof(STRUCT_VERTEX) * vertices.size());

		// Define the resource data.
		D3D11_SUBRESOURCE_DATA initData;
		ZeroMemory(&initData, sizeof(initData));
		initData.pSysMem = &vertices[0];
		device->CreateBuffer(&bufferDesc, &initData, vertexBuffer.GetAddressOf());
	}

	template <typename STRUCT_VERTEX>
	void RenderMesh<STRUCT_VERTEX>::SetIndices(const std::vector<unsigned>& indices)
	{
		this->indices = indices;

		D3D11_BUFFER_DESC bufferDesc;
		// Fill in a buffer description.
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		bufferDesc.ByteWidth = (UINT)(sizeof(unsigned int) * indices.size());
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		// Define the resource data.
		D3D11_SUBRESOURCE_DATA initData;
		ZeroMemory(&initData, sizeof(initData));
		initData.pSysMem = &indices[0];
		device->CreateBuffer(&bufferDesc, &initData, indexBuffer.GetAddressOf());
	}

	template <typename STRUCT_VERTEX>
	RenderMesh<STRUCT_VERTEX>* const RenderMesh<STRUCT_VERTEX>::Init(ID3D11Device* const device, ID3D11DeviceContext* const deviceContext)
	{
		RenderMesh<STRUCT_VERTEX>* instnace = new RenderMesh<STRUCT_VERTEX>;
		instnace->device = device;
		instnace->deviceContext = deviceContext;
		return instnace;
	}

	template <typename STRUCT_VERTEX>
	void RenderMesh<STRUCT_VERTEX>::Release(RenderMesh<STRUCT_VERTEX>** const renderMesh)
	{
		delete *renderMesh;
		*renderMesh = nullptr;
	}

	template <typename STRUCT_VERTEX>
	void RenderMesh<STRUCT_VERTEX>::Render()
	{
		UINT stride = sizeof(STRUCT_VERTEX);
		UINT offset = 0;

		deviceContext->IASetPrimitiveTopology(topology);
		deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
		deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}
}