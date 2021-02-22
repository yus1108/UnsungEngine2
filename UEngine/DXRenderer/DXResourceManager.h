#pragma once
#include "dxrframework.h"

namespace UEngine
{
	namespace DXRenderer
	{
		class DXResourceManager
		{
#pragma region Singleton
		public:
			static DXResourceManager* Get() { return &instance; }

		private:
			DXResourceManager() = default;
			~DXResourceManager() { Release(); }
			static DXResourceManager instance;
#pragma endregion

		private:
			std::unordered_map<std::string, DXShader*> shaders;
			std::unordered_map<std::string, DXRenderMesh*> renderMeshes;
			std::unordered_map<std::string, DXConstantBuffer*> constantBuffers;

		public:

			void SetShaders(std::string resource_name, DXShader* shader);
			void SetRenderMesh(std::string resource_name, DXRenderMesh* renderMesh);
			void SetConstantBuffer(std::string resource_name, DXConstantBuffer* constantBuffer);

			DXShader* GetShaders(std::string resource_name) { return shaders[resource_name]; }
			DXRenderMesh* GetRenderMesh(std::string resource_name) { return renderMeshes[resource_name]; }
			DXConstantBuffer* GetConstantBuffer(std::string resource_name) { return constantBuffers[resource_name]; }

			void Init();
			void InitShader();
			void InitRenderMesh();
			void InitConstantBuffer();

		private:
			void Release()
			{
				for (auto resource : shaders)
					DXShader::Release(&resource.second);
				for (auto resource : renderMeshes)
					DXRenderMesh::Release(&resource.second);
				for (auto resource : constantBuffers)
					DXConstantBuffer::Release(&resource.second);
			}
		};
	}
}