#include "dxrframework.h"
#include "DXRenderObject.h"

namespace UEngine
{
	namespace DXRenderer
	{
		UINT DXRenderObject::NextObjectID = 0;

		DXRenderObject* const DXRenderObject::Instantiate(std::string renderMesh, std::string shader)
		{
			auto resourceManager = DXRenderer::Get()->ResourceManager;
			DXRenderObject* instance = new DXRenderObject;
			instance->renderMesh = resourceManager->GetRenderMesh(renderMesh);
			instance->shader = resourceManager->GetShaders(shader);
			instance->objectID = NextObjectID++;
			instance->objectName = renderMesh + shader;
			return instance;
		}

		void DXRenderObject::Release(DXRenderObject* const renderObject)
		{
			delete renderObject;
		}

		void DXRenderObject::Set(ID3D11DeviceContext* const deviceContext)
		{
			if (!renderMesh || !shader) return;
			renderMesh->Set(deviceContext);
			shader->Set(deviceContext);
		}

		void DXRenderObject::Draw(ID3D11DeviceContext* const deviceContext)
		{
			if (!renderMesh || !shader) return;
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
