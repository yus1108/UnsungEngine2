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
			std::unordered_map<std::string, std::vector<SIMPLE_VERTEX>> vertexInfo;
			/*
				key: std::string - typeid(cpu_buffer_struct).raw_name()
				value: CONSTANT_BUFFER_DESC - description to create a buffer
			*/
			std::unordered_map<std::string, CONSTANT_BUFFER_DESC> constantBuffers;

		public:

			void SetShaders(std::string resource_name, DXShader* shader);
			void SetRenderMesh(std::string resource_name, DXRenderMesh* renderMesh);
			void SetConstantBuffer(std::string resource_name, CONSTANT_BUFFER_DESC constantBuffer);

			DXShader* GetShaders(std::string resource_name) { return shaders[resource_name]; }
			DXRenderMesh* GetRenderMesh(std::string resource_name) { return renderMeshes[resource_name]; }
			// resource_name : typeid(cpu_buffer_struct).raw_name()
			CONSTANT_BUFFER_DESC GetConstantBuffer(std::string resource_name) { return constantBuffers[resource_name]; }
			std::vector<SIMPLE_VERTEX> GetVertices(std::string resource_name) { return vertexInfo[resource_name]; }

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
					delete resource.second.StartSlots;
				vertexInfo.clear();
			}
		};
	}
}