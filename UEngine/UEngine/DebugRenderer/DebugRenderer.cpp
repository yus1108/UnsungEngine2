#include "UEngine.h"
#include "DebugRenderer.h"

namespace UEngine
{
	DebugRenderer::~DebugRenderer()
	{
		if (gpu_side_buffer)
			gpu_side_buffer->Release();

		if (viewContext)
			delete viewContext;
	}

	void DebugRenderer::Init(ID3D11Device* _device, ID3D11DeviceContext* _immediateContext)
	{
		if (gpu_side_buffer)
			gpu_side_buffer->Release();
		if (viewContext)
			delete viewContext;


		// create all buffers
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.ByteWidth = (UINT)(sizeof(UEngine::DebugRenderPoint) * MAX_VERTS);
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		device = _device;
		m_pImmediateContext = _immediateContext;
		device->CreateBuffer(&bufferDesc, nullptr, &gpu_side_buffer);
		ZeroMemory(&cpu_side_buffer, sizeof(cpu_side_buffer));

		// ViewContext Creation
		viewContext = new DXRenderer::ViewContext;
		auto clientSize = WinApplication::Get()->GetClientPixelSize();
		DXRenderer::Get()->InitRenderViewContext
		(
			&viewContext,
			clientSize.x,
			clientSize.y,
			DXRenderer::Get()->GetDescription().EnableDepthStencil,
			DXRenderer::Get()->GetDescription().MultisampleDesc
		);

		shader = DXRenderer::Get()->ResourceManager->GetResource<DXRenderer::DXShader>("debug");
	}

	void DebugRenderer::Add_line(UEngine::DebugRenderPoint a, UEngine::DebugRenderPoint b)
	{
		if (vert_count + 1 == MAX_VERTS) throw std::out_of_range("Cannot add more vertices!");
		if (vert_count + 2 == MAX_VERTS) throw std::out_of_range("Cannot add more vertices!");

		cpu_side_buffer[vert_count++] = a;
		cpu_side_buffer[vert_count++] = b;
	}

	void DebugRenderer::Add_line(UEngine::Math::Physics2D::PointCoord a, UEngine::Math::Physics2D::PointCoord b, UEngine::Color color)
	{
		auto pA = UEngine::DebugRenderPoint(a, color);
		auto pB = UEngine::DebugRenderPoint(b, color);
		Add_line(pA, pB);
	}

	void DebugRenderer::Add_line(UEngine::DebugRenderLine line)
	{
		auto pA = line.GetStartPoint();
		auto pB = line.GetEndPoint();
		Add_line(pA, pB);
	}

	void DebugRenderer::Add_line(UEngine::Math::Physics2D::LineCoords line, UEngine::Color color)
	{
		Add_line(line[0], line[1], color);
	}

	void DebugRenderer::Add_Triangle(UEngine::Math::Physics2D::TriangleCoords triangle, UEngine::Color color)
	{
		for (size_t i = 1; i <= 6; i++)
			if (vert_count + i == MAX_VERTS) throw std::out_of_range("Cannot add more vertices!");
		Add_line(triangle[0], triangle[1], color);
		Add_line(triangle[1], triangle[2], color);
		Add_line(triangle[2], triangle[0], color);
	}

	void DebugRenderer::Add_Rectangle(UEngine::Math::Physics2D::AABB aabb, UEngine::Color color)
	{
		for (size_t i = 1; i <= 8; i++)
			if (vert_count + i == MAX_VERTS) throw std::out_of_range("Cannot add more vertices!");

		UEngine::DebugRenderPoint debugVertices[] =
		{
			UEngine::DebugRenderPoint(Vector2(aabb.left, aabb.bottom), color),
			UEngine::DebugRenderPoint(Vector2(aabb.left, aabb.top), color),
			UEngine::DebugRenderPoint(Vector2(aabb.right, aabb.top), color),
			UEngine::DebugRenderPoint(Vector2(aabb.right, aabb.bottom), color),
		};
		Add_line(debugVertices[0], debugVertices[1]);
		Add_line(debugVertices[1], debugVertices[2]);
		Add_line(debugVertices[2], debugVertices[3]);
		Add_line(debugVertices[3], debugVertices[0]);
	}

	void DebugRenderer::Add_Circle(UEngine::Math::Physics2D::CircleCoord circle, UEngine::Color color)
	{
		unsigned slice = 360;
		for (size_t i = 1; i <= slice * 2; i++)
			if (vert_count + i == MAX_VERTS) throw std::out_of_range("Cannot add more vertices!");
		std::vector<UEngine::DebugRenderPoint> vertices;
		vertices.reserve(slice);

		float radian = Utility::UMath::PI * 2.0f / slice;
		for (size_t i = 0; i < slice; i++)
		{
			auto point = Vector2
			(
				circle.radius * cos(radian * (slice - i)),
				circle.radius * sin(radian * (slice - i))
			);
			vertices.push_back(UEngine::DebugRenderPoint(point + circle.center, color));
		}
		for (size_t i = 0; i < slice - 1; i++)
			Add_line(vertices[i], vertices[i + 1]);
		Add_line(vertices[slice - 1], vertices[0]);
	}

	void DebugRenderer::Add_Axis(Matrix worldMatrix)
	{
		for (size_t i = 1; i <= 6; i++)
			if (vert_count + i == MAX_VERTS) throw std::out_of_range("Cannot add more vertices!");

		DebugRenderPoint center, right, up, front;
		DirectX::XMVECTOR vCenter, vRight, vUp, vFront;
		vCenter = worldMatrix.r[3];
		vRight = DirectX::XMVectorAdd(vCenter, worldMatrix.r[0]);
		vUp = DirectX::XMVectorAdd(vCenter, worldMatrix.r[1]);
		vFront = DirectX::XMVectorAdd(vCenter, worldMatrix.r[2]);

		center.position = Vector3(vCenter);;
		right.position = Vector3(vRight);;
		up.position = Vector3(vUp);;
		front.position = Vector3(vFront);;

		center.color = Color{ 1, 0, 0, 1 };
		right.color = Color{ 1, 0, 0, 1 };
		up.color = Color{ 0, 1, 0, 1 };
		front.color = Color{ 0, 0, 1, 1 };
		cpu_side_buffer[vert_count++] = center;
		cpu_side_buffer[vert_count++] = right;
		center.color = Color{ 0, 1, 0, 1 };
		cpu_side_buffer[vert_count++] = center;
		cpu_side_buffer[vert_count++] = up;
		center.color = Color{ 0, 0, 1, 1 };
		cpu_side_buffer[vert_count++] = center;
		cpu_side_buffer[vert_count++] = front;

	}

	void DebugRenderer::Flush(DXRenderer::DXConstantBuffer* mainCameraBuffer)
	{
		// set texture info
		viewContext->DeviceContext->ClearRenderTargetView(viewContext->RenderTargetView.Get(), DirectX::Colors::Transparent);
		if (viewContext->DepthStencilView) viewContext->DeviceContext->ClearDepthStencilView(viewContext->DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
		if (vert_count == 0)
		{
			viewContext->CommandList.ReleaseAndGetAddressOf();
			viewContext->DeviceContext->FinishCommandList(true, viewContext->CommandList.GetAddressOf());
			return;
		}
		ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
		viewContext->DeviceContext->Map(gpu_side_buffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &mappedResource);
		memcpy(mappedResource.pData, &cpu_side_buffer, sizeof(UEngine::DebugRenderPoint) * vert_count);
		viewContext->DeviceContext->Unmap(gpu_side_buffer, 0);

		shader->Set(viewContext->DeviceContext.Get());
		mainCameraBuffer->Set(viewContext->DeviceContext.Get());

		viewContext->DeviceContext->IASetVertexBuffers(0, 1, &gpu_side_buffer, &stride, &offset);
		viewContext->DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
		viewContext->DeviceContext->RSSetViewports(1, &viewContext->Viewport);
		viewContext->DeviceContext->OMSetDepthStencilState(viewContext->DepthStencilState.Get(), 1);
		viewContext->DeviceContext->OMSetRenderTargets(1, viewContext->RenderTargetView.GetAddressOf(), viewContext->DepthStencilView.Get());
		viewContext->DeviceContext->Draw(vert_count, 0);
		viewContext->CommandList.ReleaseAndGetAddressOf();
		viewContext->DeviceContext->FinishCommandList(true, viewContext->CommandList.GetAddressOf());

		vert_count = 0;
	}

	void DebugRenderer::Render(ID3D11DeviceContext* deviceContext)
	{
		if (viewContext->CommandList == nullptr) return;
		deviceContext->ExecuteCommandList(viewContext->CommandList.Get(), true);

		deviceContext->ResolveSubresource
		(
			(ID3D11Resource*)viewContext->OutputTexture2D.Get(),
			D3D11CalcSubresource(0, 0, 1),
			(ID3D11Resource*)viewContext->RenderTargetViewTexture2D.Get(),
			D3D11CalcSubresource(0, 0, 1),
			DXGI_FORMAT_R32G32B32A32_FLOAT
		);
	}

	ID3D11ShaderResourceView** DebugRenderer::GetAddressOfViewResource()
	{
		return viewContext->OutputShaderResourceView.GetAddressOf();
	}

	ID3D11ShaderResourceView* DebugRenderer::GetViewResource()
	{
		return viewContext->OutputShaderResourceView.Get();
	}
}