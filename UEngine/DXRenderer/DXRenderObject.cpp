#include "dxrframework.h"
#include "DXRenderObject.h"

namespace UEngine
{
	namespace DXRenderer
	{

		DXRenderObject* const DXRenderObject::Instantiate
		(
			DXRenderMesh* renderMesh,
			DXShader* shader
		)
		{
			DXRenderObject* instance = new DXRenderObject;
			instance->renderMesh = renderMesh;
			instance->shader = shader;

			return instance;
		}

		void DXRenderObject::Release(DXRenderObject** const renderObject)
		{
			delete* renderObject;
			*renderObject = nullptr;
		}

		void DXRenderObject::Release(DXRenderObject* const renderObject)
		{
			delete renderObject;
		}

		void DXRenderObject::AddConstantBuffer(const std::string bufferName, DXConstantBuffer* const constantBuffer)
		{
			constantBuffers[bufferName] = constantBuffer;
		}

		DXConstantBuffer* const DXRenderObject::RemoveConstantBuffer(const std::string bufferName)
		{
			auto buffer = constantBuffers[bufferName];
			constantBuffers.erase(bufferName);
			return buffer;
		}

		void DXRenderObject::CBAttachData(const std::string bufferName, const void* data, size_t _Size)
		{
			constantBuffers[bufferName]->AttachData(data, _Size);
		}

		void DXRenderObject::CBUpdate(ID3D11DeviceContext* const deviceContext, const std::string bufferName)
		{
			constantBuffers[bufferName]->Update(deviceContext);
		}

		void DXRenderObject::CBUpdateAll(ID3D11DeviceContext* const deviceContext)
		{
			for (auto buffer : constantBuffers)
				buffer.second->Update(deviceContext);
		}

		void DXRenderObject::Set(ID3D11DeviceContext* const deviceContext)
		{
			renderMesh->Set(deviceContext);
			shader->Set(deviceContext);
			for (auto buffer : constantBuffers)
				buffer.second->Set(deviceContext);
		}

		void DXRenderObject::Draw(ID3D11DeviceContext* const deviceContext)
		{
			UINT indexCount = renderMesh->GetIndexCount();
			if (indexCount == 0)
			{
				deviceContext->Draw(renderMesh->GetVertexCount(), 0);
				return;
			}
			deviceContext->DrawIndexed(indexCount, 0, 0);
		}
	}
}
