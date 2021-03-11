#pragma once

namespace UEngine
{
	namespace DXRenderer
	{
		class DXRenderMesh final
		{
		private:
			DXRenderMesh() {}
			~DXRenderMesh() = default;
		private:
			UINT stride{ 0 };
			std::vector<SIMPLE_VERTEX> vertices;
			UINT vertexCount{ 0 };
			std::vector<unsigned> indices;
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
			const std::vector<SIMPLE_VERTEX>& GetVertices() { return vertices; }
			unsigned GetVertexCount() { return vertexCount; }
			const std::vector<unsigned>& GetIndices() { return indices; }
			unsigned GetIndexCount() { return indexCount; }

			// Initialize to Draw line strip with only vertices
			static DXRenderMesh* const Instantiate
			(
				ID3D11Device* const device,
				const std::vector<SIMPLE_VERTEX>& vertices,
				D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP
			);

			// Initialize to Draw with vertices, indices, and the given topology
			static DXRenderMesh* const Instantiate
			(
				ID3D11Device* const device, 
				const std::vector<SIMPLE_VERTEX>& vertices,
				const std::vector<unsigned>& indices,
				D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
			);
			static void Release(DXRenderMesh** const renderMesh);
			static void Release(DXRenderMesh* const renderMesh);
			void Set(ID3D11DeviceContext* const deviceContext);
			void Draw(ID3D11DeviceContext* const deviceContext);
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
	}
}