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

		shader = DXRenderer::DXResourceManager::Get()->GetShaders("debug");
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

	//void DebugRenderer::Add_AABB(AABB aabb, DirectX::XMFLOAT4 color)
	//{
	//	UEngine::DebugVertex vertices[24];
	//	DirectX::XMFLOAT2 xaxis = aabb.GetXAxis();
	//	DirectX::XMFLOAT2 yaxis = aabb.GetYAxis();
	//	DirectX::XMFLOAT2 zaxis = aabb.GetZAxis();
	//	// bottom
	//	vertices[0].pos = DirectX::XMFLOAT3(xaxis.x, yaxis.x, zaxis.x);
	//	vertices[1].pos = DirectX::XMFLOAT3(xaxis.y, yaxis.x, zaxis.x);
	//	vertices[2].pos = DirectX::XMFLOAT3(xaxis.x, yaxis.x, zaxis.y);
	//	vertices[3].pos = DirectX::XMFLOAT3(xaxis.y, yaxis.x, zaxis.y);
	//	vertices[4].pos = DirectX::XMFLOAT3(xaxis.x, yaxis.x, zaxis.x);
	//	vertices[5].pos = DirectX::XMFLOAT3(xaxis.x, yaxis.x, zaxis.y);
	//	vertices[6].pos = DirectX::XMFLOAT3(xaxis.y, yaxis.x, zaxis.x);
	//	vertices[7].pos = DirectX::XMFLOAT3(xaxis.y, yaxis.x, zaxis.y);

	//	// middle
	//	vertices[8].pos = DirectX::XMFLOAT3(xaxis.x, yaxis.x, zaxis.x);
	//	vertices[9].pos = DirectX::XMFLOAT3(xaxis.x, yaxis.y, zaxis.x);
	//	vertices[10].pos = DirectX::XMFLOAT3(xaxis.y, yaxis.x, zaxis.x);
	//	vertices[11].pos = DirectX::XMFLOAT3(xaxis.y, yaxis.y, zaxis.x);
	//	vertices[12].pos = DirectX::XMFLOAT3(xaxis.y, yaxis.x, zaxis.y);
	//	vertices[13].pos = DirectX::XMFLOAT3(xaxis.y, yaxis.y, zaxis.y);
	//	vertices[14].pos = DirectX::XMFLOAT3(xaxis.x, yaxis.x, zaxis.y);
	//	vertices[15].pos = DirectX::XMFLOAT3(xaxis.x, yaxis.y, zaxis.y);

	//	// top
	//	vertices[16].pos = DirectX::XMFLOAT3(xaxis.x, yaxis.y, zaxis.x);
	//	vertices[17].pos = DirectX::XMFLOAT3(xaxis.y, yaxis.y, zaxis.x);
	//	vertices[18].pos = DirectX::XMFLOAT3(xaxis.x, yaxis.y, zaxis.y);
	//	vertices[19].pos = DirectX::XMFLOAT3(xaxis.y, yaxis.y, zaxis.y);
	//	vertices[20].pos = DirectX::XMFLOAT3(xaxis.x, yaxis.y, zaxis.x);
	//	vertices[21].pos = DirectX::XMFLOAT3(xaxis.x, yaxis.y, zaxis.y);
	//	vertices[22].pos = DirectX::XMFLOAT3(xaxis.y, yaxis.y, zaxis.x);
	//	vertices[23].pos = DirectX::XMFLOAT3(xaxis.y, yaxis.y, zaxis.y);

	//	for (unsigned int i = 0; i < 24; i++) {
	//		cpu_side_buffer[vert_count] = vertices[i];
	//		cpu_side_buffer[vert_count++].color = color;
	//	}
	//}
	//void DebugRenderer::Add_OOBB(OOBB* oobb, DirectX::XMFLOAT4 color)
	//{
	//	DirectX::XMVECTOR vertices[24];
	//	DirectX::XMFLOAT2 xaxis = oobb->GetXAxis();
	//	DirectX::XMFLOAT2 yaxis = oobb->GetYAxis();
	//	DirectX::XMFLOAT2 zaxis = oobb->GetZAxis();
	//	// bottom
	//	vertices[0] = DirectX::XMVectorSet(xaxis.x, yaxis.x, zaxis.x, 1);
	//	vertices[1] = DirectX::XMVectorSet(xaxis.y, yaxis.x, zaxis.x, 1);
	//	vertices[2] = DirectX::XMVectorSet(xaxis.x, yaxis.x, zaxis.y, 1);
	//	vertices[3] = DirectX::XMVectorSet(xaxis.y, yaxis.x, zaxis.y, 1);
	//	vertices[4] = DirectX::XMVectorSet(xaxis.x, yaxis.x, zaxis.x, 1);
	//	vertices[5] = DirectX::XMVectorSet(xaxis.x, yaxis.x, zaxis.y, 1);
	//	vertices[6] = DirectX::XMVectorSet(xaxis.y, yaxis.x, zaxis.x, 1);
	//	vertices[7] = DirectX::XMVectorSet(xaxis.y, yaxis.x, zaxis.y, 1);

	//	// middle
	//	vertices[8] = DirectX::XMVectorSet(xaxis.x, yaxis.x, zaxis.x, 1);
	//	vertices[9] = DirectX::XMVectorSet(xaxis.x, yaxis.y, zaxis.x, 1);
	//	vertices[10] = DirectX::XMVectorSet(xaxis.y, yaxis.x, zaxis.x, 1);
	//	vertices[11] = DirectX::XMVectorSet(xaxis.y, yaxis.y, zaxis.x, 1);
	//	vertices[12] = DirectX::XMVectorSet(xaxis.y, yaxis.x, zaxis.y, 1);
	//	vertices[13] = DirectX::XMVectorSet(xaxis.y, yaxis.y, zaxis.y, 1);
	//	vertices[14] = DirectX::XMVectorSet(xaxis.x, yaxis.x, zaxis.y, 1);
	//	vertices[15] = DirectX::XMVectorSet(xaxis.x, yaxis.y, zaxis.y, 1);

	//	// top
	//	vertices[16] = DirectX::XMVectorSet(xaxis.x, yaxis.y, zaxis.x, 1);
	//	vertices[17] = DirectX::XMVectorSet(xaxis.y, yaxis.y, zaxis.x, 1);
	//	vertices[18] = DirectX::XMVectorSet(xaxis.x, yaxis.y, zaxis.y, 1);
	//	vertices[19] = DirectX::XMVectorSet(xaxis.y, yaxis.y, zaxis.y, 1);
	//	vertices[20] = DirectX::XMVectorSet(xaxis.x, yaxis.y, zaxis.x, 1);
	//	vertices[21] = DirectX::XMVectorSet(xaxis.x, yaxis.y, zaxis.y, 1);
	//	vertices[22] = DirectX::XMVectorSet(xaxis.y, yaxis.y, zaxis.x, 1);
	//	vertices[23] = DirectX::XMVectorSet(xaxis.y, yaxis.y, zaxis.y, 1);

	//	for (unsigned int i = 0; i < 24; i++) {
	//		vertices[i] = DirectX::XMVector4Transform(vertices[i], oobb->GetWorldMat());
	//		DirectX::XMStoreFloat3(&cpu_side_buffer[vert_count].pos, vertices[i]);
	//		cpu_side_buffer[vert_count++].color = color;
	//	}
	//}
	//void DebugRenderer::Add_Frustum(Frustum frustum, DirectX::XMFLOAT4 color) {
	//	DirectX::XMVECTOR vertices[8];

	//	using namespace DirectX;
	//	XMMATRIX normalizedMat = frustum.GetWorldMat();
	//	normalizedMat.r[0] = XMVector3Normalize(normalizedMat.r[0]);
	//	normalizedMat.r[1] = XMVector3Normalize(normalizedMat.r[1]);
	//	normalizedMat.r[2] = XMVector3Normalize(normalizedMat.r[2]);
	//	for (unsigned int i = 0; i < 8; i++) {
	//		vertices[i] = DirectX::XMVector4Transform(frustum.GetVertex(i), normalizedMat);
	//	}

	//	DirectX::XMStoreFloat3(&cpu_side_buffer[vert_count].pos, vertices[0]);
	//	cpu_side_buffer[vert_count++].color = color;
	//	DirectX::XMStoreFloat3(&cpu_side_buffer[vert_count].pos, vertices[1]);
	//	cpu_side_buffer[vert_count++].color = color;

	//	DirectX::XMStoreFloat3(&cpu_side_buffer[vert_count].pos, vertices[0]);
	//	cpu_side_buffer[vert_count++].color = color;
	//	DirectX::XMStoreFloat3(&cpu_side_buffer[vert_count].pos, vertices[2]);
	//	cpu_side_buffer[vert_count++].color = color;

	//	DirectX::XMStoreFloat3(&cpu_side_buffer[vert_count].pos, vertices[1]);
	//	cpu_side_buffer[vert_count++].color = color;
	//	DirectX::XMStoreFloat3(&cpu_side_buffer[vert_count].pos, vertices[3]);
	//	cpu_side_buffer[vert_count++].color = color;

	//	DirectX::XMStoreFloat3(&cpu_side_buffer[vert_count].pos, vertices[2]);
	//	cpu_side_buffer[vert_count++].color = color;
	//	DirectX::XMStoreFloat3(&cpu_side_buffer[vert_count].pos, vertices[3]);
	//	cpu_side_buffer[vert_count++].color = color;

	//	DirectX::XMStoreFloat3(&cpu_side_buffer[vert_count].pos, vertices[0]);
	//	cpu_side_buffer[vert_count++].color = color;
	//	DirectX::XMStoreFloat3(&cpu_side_buffer[vert_count].pos, vertices[4]);
	//	cpu_side_buffer[vert_count++].color = color;

	//	DirectX::XMStoreFloat3(&cpu_side_buffer[vert_count].pos, vertices[1]);
	//	cpu_side_buffer[vert_count++].color = color;
	//	DirectX::XMStoreFloat3(&cpu_side_buffer[vert_count].pos, vertices[5]);
	//	cpu_side_buffer[vert_count++].color = color;

	//	DirectX::XMStoreFloat3(&cpu_side_buffer[vert_count].pos, vertices[4]);
	//	cpu_side_buffer[vert_count++].color = color;
	//	DirectX::XMStoreFloat3(&cpu_side_buffer[vert_count].pos, vertices[5]);
	//	cpu_side_buffer[vert_count++].color = color;

	//	DirectX::XMStoreFloat3(&cpu_side_buffer[vert_count].pos, vertices[2]);
	//	cpu_side_buffer[vert_count++].color = color;
	//	DirectX::XMStoreFloat3(&cpu_side_buffer[vert_count].pos, vertices[6]);
	//	cpu_side_buffer[vert_count++].color = color;

	//	DirectX::XMStoreFloat3(&cpu_side_buffer[vert_count].pos, vertices[3]);
	//	cpu_side_buffer[vert_count++].color = color;
	//	DirectX::XMStoreFloat3(&cpu_side_buffer[vert_count].pos, vertices[7]);
	//	cpu_side_buffer[vert_count++].color = color;

	//	DirectX::XMStoreFloat3(&cpu_side_buffer[vert_count].pos, vertices[6]);
	//	cpu_side_buffer[vert_count++].color = color;
	//	DirectX::XMStoreFloat3(&cpu_side_buffer[vert_count].pos, vertices[7]);
	//	cpu_side_buffer[vert_count++].color = color;

	//	DirectX::XMStoreFloat3(&cpu_side_buffer[vert_count].pos, vertices[4]);
	//	cpu_side_buffer[vert_count++].color = color;
	//	DirectX::XMStoreFloat3(&cpu_side_buffer[vert_count].pos, vertices[6]);
	//	cpu_side_buffer[vert_count++].color = color;

	//	DirectX::XMStoreFloat3(&cpu_side_buffer[vert_count].pos, vertices[5]);
	//	cpu_side_buffer[vert_count++].color = color;
	//	DirectX::XMStoreFloat3(&cpu_side_buffer[vert_count].pos, vertices[7]);
	//	cpu_side_buffer[vert_count++].color = color;
	//}
	//void DebugRenderer::Add_UI_AABB(AABB aabb, DirectX::XMFLOAT4 color)
	//{
	//	DirectX::XMVECTOR pos[24];
	//	UEngine::DebugVertex vertices[24];
	//	DirectX::XMFLOAT2 xaxis = aabb.GetXAxis();
	//	DirectX::XMFLOAT2 yaxis = aabb.GetYAxis();
	//	DirectX::XMFLOAT2 zaxis = aabb.GetZAxis();
	//	// bottom
	//	pos[0] = DirectX::XMVectorSet(xaxis.x, yaxis.x, zaxis.x, 1);
	//	pos[1] = DirectX::XMVectorSet(xaxis.y, yaxis.x, zaxis.x, 0);
	//	pos[2] = DirectX::XMVectorSet(xaxis.x, yaxis.x, zaxis.y, 0);
	//	pos[3] = DirectX::XMVectorSet(xaxis.y, yaxis.x, zaxis.y, 0);
	//	pos[4] = DirectX::XMVectorSet(xaxis.x, yaxis.x, zaxis.x, 0);
	//	pos[5] = DirectX::XMVectorSet(xaxis.x, yaxis.x, zaxis.y, 0);
	//	pos[6] = DirectX::XMVectorSet(xaxis.y, yaxis.x, zaxis.x, 0);
	//	pos[7] = DirectX::XMVectorSet(xaxis.y, yaxis.x, zaxis.y, 0);

	//	// middle
	//	pos[8] = DirectX::XMVectorSet(xaxis.x, yaxis.x, zaxis.x, 1);
	//	pos[9] = DirectX::XMVectorSet(xaxis.x, yaxis.y, zaxis.x, 1);
	//	pos[10] = DirectX::XMVectorSet(xaxis.y, yaxis.x, zaxis.x, 1);
	//	pos[11] = DirectX::XMVectorSet(xaxis.y, yaxis.y, zaxis.x, 1);
	//	pos[12] = DirectX::XMVectorSet(xaxis.y, yaxis.x, zaxis.y, 1);
	//	pos[13] = DirectX::XMVectorSet(xaxis.y, yaxis.y, zaxis.y, 1);
	//	pos[14] = DirectX::XMVectorSet(xaxis.x, yaxis.x, zaxis.y, 1);
	//	pos[15] = DirectX::XMVectorSet(xaxis.x, yaxis.y, zaxis.y, 1);

	//	// top
	//	pos[16] = DirectX::XMVectorSet(xaxis.x, yaxis.y, zaxis.x, 1);
	//	pos[17] = DirectX::XMVectorSet(xaxis.y, yaxis.y, zaxis.x, 1);
	//	pos[18] = DirectX::XMVectorSet(xaxis.x, yaxis.y, zaxis.y, 1);
	//	pos[19] = DirectX::XMVectorSet(xaxis.y, yaxis.y, zaxis.y, 1);
	//	pos[20] = DirectX::XMVectorSet(xaxis.x, yaxis.y, zaxis.x, 1);
	//	pos[21] = DirectX::XMVectorSet(xaxis.x, yaxis.y, zaxis.y, 1);
	//	pos[22] = DirectX::XMVectorSet(xaxis.y, yaxis.y, zaxis.x, 1);
	//	pos[23] = DirectX::XMVectorSet(xaxis.y, yaxis.y, zaxis.y, 1);

	//	for (unsigned int i = 0; i < 24; i++) {
	//		DirectX::XMMATRIX perspective = DirectX::XMMatrixTranspose(gameState.renderer->m_pCameras[0]->GetSceneToShader().perspectivMat);
	//		pos[i] = DirectX::XMVector3Transform(pos[i], gameState.renderer->m_pCameras[0]->GetOriginalView());
	//		pos[i] = DirectX::XMVector3Transform(pos[i], DirectX::XMMatrixInverse(&DirectX::XMMatrixDeterminant(perspective), perspective));
	//		vertices[i].pos = DirectX::XMFLOAT3(pos[i].m128_f32[0], pos[i].m128_f32[1], pos[i].m128_f32[2]);
	//		cpu_side_buffer[vert_count] = vertices[i];
	//		cpu_side_buffer[vert_count++].color = color;
	//	}
	//}

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
		if (vert_count == 0) return;
		ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
		m_pImmediateContext->Map(gpu_side_buffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &mappedResource);
		memcpy(mappedResource.pData, &cpu_side_buffer, sizeof(UEngine::DebugRenderPoint) * vert_count);
		m_pImmediateContext->Unmap(gpu_side_buffer, 0);

		// set texture info
		viewContext->DeviceContext->ClearRenderTargetView(viewContext->RenderTargetView.Get(), DirectX::Colors::Transparent);
		if (viewContext->DepthStencilView) viewContext->DeviceContext->ClearDepthStencilView(viewContext->DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

		shader->Set(viewContext->DeviceContext.Get());
		mainCameraBuffer->Set(viewContext->DeviceContext.Get());

		viewContext->DeviceContext->IASetVertexBuffers(0, 1, &gpu_side_buffer, &stride, &offset);
		viewContext->DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
		viewContext->DeviceContext->RSSetViewports(1, &viewContext->Viewport);
		viewContext->DeviceContext->OMSetDepthStencilState(viewContext->DepthStencilState.Get(), 1);
		viewContext->DeviceContext->OMSetRenderTargets(1, viewContext->RenderTargetView.GetAddressOf(), viewContext->DepthStencilView.Get());
		viewContext->DeviceContext->Draw(vert_count, 0);
		viewContext->DeviceContext->FinishCommandList(true, viewContext->CommandList.GetAddressOf());
	}

	ID3D11ShaderResourceView** DebugRenderer::GetViewResource()
	{
		if (vert_count == 0) return nullptr;
		m_pImmediateContext->ExecuteCommandList(viewContext->CommandList.Get(), true);
		viewContext->CommandList.ReleaseAndGetAddressOf();

		m_pImmediateContext->ResolveSubresource
		(
			(ID3D11Resource*)viewContext->OutputTexture2D.Get(),
			D3D11CalcSubresource(0, 0, 1),
			(ID3D11Resource*)viewContext->RenderTargetViewTexture2D.Get(),
			D3D11CalcSubresource(0, 0, 1),
			DXGI_FORMAT_R32G32B32A32_FLOAT
		);

		vert_count = 0;
		return viewContext->OutputShaderResourceView.GetAddressOf();
	}


}
