#include "dxrframework.h"
#include "DXRenderObject.h"

namespace UEngine
{
	namespace DXRenderer
	{
		UINT DXRenderObject::NextObjectNumber = 0;

		DXRenderObject* const DXRenderObject::Instantiate(DXRenderMesh* renderMesh, DXShader* shader)
		{
			DXRenderObject* instance = new DXRenderObject;
			instance->renderMesh = renderMesh;
			instance->shader = shader;
			instance->objectNumber = NextObjectNumber++;

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
