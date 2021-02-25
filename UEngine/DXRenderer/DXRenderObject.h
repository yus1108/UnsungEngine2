#pragma once

namespace UEngine
{
	namespace DXRenderer
	{
		class DXRenderObject final
		{
			friend class DXRenderer;
		private:
			DXRenderObject() {}
			~DXRenderObject() = default;
		private:
			UINT objectID{ 0 };
			std::string objectName;
			class DXRenderMesh* renderMesh{ nullptr };
			class DXShader* shader{ nullptr };

#pragma region RESOURCE_CREATION
			friend class DXRenderObjectPool;
			static UINT NextObjectID;
			static DXRenderObject* const Instantiate(std::string renderMesh, std::string shader);
			static void Release(DXRenderObject* const renderObject);
#pragma endregion

		public:
			DXRenderMesh* const GetRenderMesh() { return renderMesh; }
			DXShader* const GetShader() { return shader; }
			UINT GetID() { return objectID; }
			std::string GetName() { return objectName; }

			void const SetRenderMesh(DXRenderMesh* renderMesh) { this->renderMesh = renderMesh; }
			void const SetShader(DXShader* shader) { this->shader = shader; }

			void Set(ID3D11DeviceContext* const deviceContext);
			void Draw(ID3D11DeviceContext* const deviceContext);
		};
	}
}

