#pragma once

namespace UEngine
{
	namespace DXRenderer
	{
		class DXRenderObject final
		{
		private:
			DXRenderObject() {}
			~DXRenderObject() = default;
		private:
			UINT objectNumber{ 0 };
			class DXRenderMesh* renderMesh{ nullptr };
			class DXShader* shader{ nullptr };
			std::list<CONSTANT_BUFFER_DESC> bufferDescs;

#pragma region POOL_CREATION
			friend class DXRenderObjectPool;
			static UINT NextObjectNumber;
			static DXRenderObject* const Instantiate
			(
				DXRenderMesh* renderMesh,
				DXShader* shader,
				std::list<CONSTANT_BUFFER_DESC> bufferDescs
			);
			static void Release(DXRenderObject* const renderObject);
#pragma endregion

		public:

			DXRenderMesh* const GetRenderMesh() { return renderMesh; }
			DXShader* const GetShader() { return shader; }

			void const SetRenderMesh(DXRenderMesh* renderMesh) { this->renderMesh = renderMesh; }
			void const SetShader(DXShader* shader) { this->shader = shader; }

			void Set(ID3D11DeviceContext* const deviceContext);
			void Draw(ID3D11DeviceContext* const deviceContext);
		};
	}
}

