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

		static const unsigned MAX_VERTS{ 500000 };
		ID3D11Buffer* gpu_side_buffer{ nullptr };
		UEngine::DebugRenderPoint cpu_side_buffer[MAX_VERTS];
		unsigned vert_count{ 0 };

		UINT stride{ sizeof(UEngine::DebugRenderPoint) };
		UINT offset{ 0 };

		D3D11_MAPPED_SUBRESOURCE mappedResource{ NULL };

	public:
		DebugRenderer() = default;
		~DebugRenderer();

		void Init(ID3D11Device* _device, ID3D11DeviceContext* _immediateContext);
		void Add_line(UEngine::DebugRenderPoint a, UEngine::DebugRenderPoint b);
		void Add_line(UEngine::Math::Physics2D::PointCoord a, UEngine::Math::Physics2D::PointCoord b, UEngine::Color color);
		void Add_line(UEngine::DebugRenderLine line);
		void Add_line(UEngine::Math::Physics2D::LineCoords line, UEngine::Color color);
		void Add_Triangle(UEngine::Math::Physics2D::TriangleCoords triangle, UEngine::Color color);
		void Add_Rectangle(UEngine::Math::Physics2D::AABB aabb, UEngine::Color color);
		void Add_Circle(UEngine::Math::Physics2D::CircleCoord circle, UEngine::Color color);
		/*void Add_OOBB(OOBB * oobb, DirectX::XMFLOAT4 color);
		void Add_Frustum(Frustum frustum, DirectX::XMFLOAT4 color);
		void Add_UI_AABB(AABB aabb, DirectX::XMFLOAT4 color);*/
		void Add_Axis(Matrix worldMatrix);
		void Flush(DXRenderer::DXConstantBuffer* mainCameraBuffer);
		void Render(ID3D11DeviceContext* deviceContext);

		ID3D11ShaderResourceView** GetAddressOfViewResource();
		ID3D11ShaderResourceView* GetViewResource();
	};
}
