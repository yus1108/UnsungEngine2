#pragma once
//#include "OOBB.h"

namespace UEngine
{
	class DebugRenderer
	{
	private:
		// taking it from graphics system
		ID3D11Device* device{ nullptr };
		ID3D11DeviceContext* m_pImmediateContext{ nullptr };

		DXRenderer::ViewContext* viewContext{ nullptr };
		DXRenderer::DXShader* shader{ nullptr };

		static const unsigned MAX_VERTS{ 100000 };
		ID3D11Buffer* gpu_side_buffer{ nullptr };
		UEngine::DebugVertex cpu_side_buffer[MAX_VERTS];
		unsigned vert_count{ 0 };

		UINT stride{ sizeof(UEngine::DebugVertex) };
		UINT offset{ 0 };

		D3D11_MAPPED_SUBRESOURCE mappedResource;

	public:
		DebugRenderer() = default;
		~DebugRenderer();

		void Init(ID3D11Device* _device, ID3D11DeviceContext* _immediateContext);
		void Add_line(UEngine::DebugVertex a, UEngine::DebugVertex b);
		/*void Add_AABB(AABB aabb, DirectX::XMFLOAT4 color);
		void Add_OOBB(OOBB * oobb, DirectX::XMFLOAT4 color);
		void Add_Frustum(Frustum frustum, DirectX::XMFLOAT4 color);
		void Add_UI_AABB(AABB aabb, DirectX::XMFLOAT4 color);*/
		//void Add_Axis(UnitNode * node);
		void Flush(DXRenderer::DXConstantBuffer* mainCameraBuffer);

		ID3D11ShaderResourceView** GetViewResource(ID3D11DeviceContext* deviceContext);
	};
}
