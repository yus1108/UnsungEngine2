#pragma once

namespace UEngine
{
	class DXRenderMesh final
	{
	private:
		DXRenderMesh() {}
		~DXRenderMesh() = default;
	private:
		UINT stride{ 0 };
		UINT vertexCount{ 0 };
		UINT indexCount{ 0 };

		D3D11_PRIMITIVE_TOPOLOGY topology{ D3D_PRIMITIVE_TOPOLOGY_UNDEFINED };
		Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	public:
		template <typename T>
		void SetVertices(ID3D11Device* const device, const T* const vertices, UINT vertexCount);
		void SetIndices(ID3D11Device* const device, const unsigned* const indices, UINT indexCount);
		void SetTopology(D3D11_PRIMITIVE_TOPOLOGY topology) { this->topology = topology; }

		unsigned GetStride() { return stride; }
		unsigned GetVertexCount() { return vertexCount; }
		unsigned GetIndexCount() { return indexCount; }

		// Initialize to Draw line strip with only vertices
		template <typename T>
		static DXRenderMesh* const Instantiate
		(
			ID3D11Device* const device,
			const T* const vertices,
			UINT vertexCount,
			D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP
		);

		// Initialize to Draw with vertices, indices, and the given topology
		template <typename T>
		static DXRenderMesh* const Instantiate
		(
			ID3D11Device* const device, 
			const T* const vertices,
			UINT vertexCount,
			const unsigned* const indices,
			UINT indexCount,
			D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
		);
		static void Release(DXRenderMesh** const renderMesh);
		void Set(ID3D11DeviceContext* const deviceContext);
	};	

	template <typename T>
	void DXRenderMesh::SetVertices(ID3D11Device* const device, const T* const vertices, UINT vertexCount)
	{
		stride = sizeof(T);
		this->vertexCount = vertexCount;

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.ByteWidth = (UINT)(sizeof(T) * vertexCount);

		// Define the resource data.
		D3D11_SUBRESOURCE_DATA initData;
		ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));
		initData.pSysMem = vertices;
		device->CreateBuffer(&bufferDesc, &initData, vertexBuffer.GetAddressOf());
	}

	template <typename T>
	DXRenderMesh* const DXRenderMesh::Instantiate
	(
		ID3D11Device* const device,
		const T* const vertices,
		UINT vertexCount,
		D3D11_PRIMITIVE_TOPOLOGY topology
	)
	{
		DXRenderMesh* instance = new DXRenderMesh;
		instance->SetVertices<T>(device, vertices, vertexCount);
		instance->SetTopology(topology);
		return instance;
	}

	template <typename T>
	DXRenderMesh* const DXRenderMesh::Instantiate
	(
		ID3D11Device* const device,
		const T* const vertices,
		UINT vertexCount,
		const unsigned* const indices,
		UINT indexCount,
		D3D11_PRIMITIVE_TOPOLOGY topology
	)
	{
		DXRenderMesh* instance = new DXRenderMesh;
		instance->SetVertices<T>(device, vertices, vertexCount);
		instance->SetIndices(device, indices, indexCount);
		instance->SetTopology(topology);
		return instance;
	}
}