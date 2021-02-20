#include "dxrframework.h"
#include "DXRenderObject.h"

UEngine::DXRenderObject* const UEngine::DXRenderObject::Instantiate
(
	DXRenderMesh* renderMesh,
	DXShader* shader,
	bool allowGCforSubComponents
)
{
	DXRenderObject* instance = new DXRenderObject;
	instance->renderMesh = renderMesh;
	instance->shader = shader;
	instance->AllowGCforSubComponents = allowGCforSubComponents;

	return instance;
}

void UEngine::DXRenderObject::Release(DXRenderObject** const renderObject)
{
	if (*renderObject && (*renderObject)->AllowGCforSubComponents)
	{
		DXRenderMesh::Release(&(*renderObject)->renderMesh);
		DXShader::Release(&(*renderObject)->shader);
		for (auto buffer : (*renderObject)->constantBuffers)
			DXConstantBuffer::Release(&buffer.second);
	}
	delete* renderObject;
	*renderObject = nullptr;
}

void UEngine::DXRenderObject::AddConstantBuffer(DXRenderer* const renderer, const std::string bufferName, size_t _Data_Size, UINT flags, UINT startSlot)
{
	constantBuffers[bufferName] = UEngine::DXConstantBuffer::Instantiate(renderer, _Data_Size, flags, startSlot);
}

void UEngine::DXRenderObject::AddConstantBuffer(const std::string bufferName, DXConstantBuffer* const constantBuffer)
{
	constantBuffers[bufferName] = constantBuffer;
}

void UEngine::DXRenderObject::CBAttachData(const std::string bufferName, const void* data, size_t _Size)
{
	constantBuffers[bufferName]->AttachData(data, _Size);
}

void UEngine::DXRenderObject::CBUpdate(ID3D11DeviceContext* const deviceContext, const std::string bufferName)
{
	constantBuffers[bufferName]->Update(deviceContext);
}

void UEngine::DXRenderObject::CBUpdateAll(ID3D11DeviceContext* const deviceContext)
{
	for (auto buffer : constantBuffers)
		buffer.second->Update(deviceContext);
}

void UEngine::DXRenderObject::Set(ID3D11DeviceContext* const deviceContext)
{
	renderMesh->Set(deviceContext);
	shader->Set(deviceContext);
	for (auto buffer : constantBuffers)
		buffer.second->Set(deviceContext);
}

void UEngine::DXRenderObject::Draw(ID3D11DeviceContext* const deviceContext)
{
	UINT indexCount = renderMesh->GetIndexCount();
	if (indexCount == 0)
	{
		deviceContext->Draw(renderMesh->GetVertexCount(), 0);
		return;
	}
	deviceContext->DrawIndexed(indexCount, 0, 0);
}