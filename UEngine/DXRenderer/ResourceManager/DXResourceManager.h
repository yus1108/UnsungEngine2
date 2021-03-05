#pragma once
#include "dxrframework.h"
#include "DXRenderObjectPool.h"

namespace UEngine
{
	namespace DXRenderer
	{
		class DXResourceManager
		{
		private:
			
			/*
				key: std::string - typeid(cpu_buffer_struct).raw_name()
				value: CONSTANT_BUFFER_DESC - description to create a buffer
			*/
			std::unordered_map<std::string, CONSTANT_BUFFER_DESC> constantBuffers;
			std::unordered_map<std::string, DXShader*> shaders;
			std::map<std::string, DXRenderMesh*> renderMeshes;
			std::map<std::string, std::vector<SIMPLE_VERTEX>> vertexInfo;

			std::unordered_map<std::string, SHARED_RENDERMESH*> shared_rendermesh;
			std::unordered_map<std::string, DXRenderMesh*> loadedRenderMeshes;
			std::unordered_map<std::string, std::vector<SIMPLE_VERTEX>> loadedVertexInfo;

			std::unordered_map<std::string, class DXScene*> scenes;

		public:
			DXResourceManager() = default;
			~DXResourceManager() { Release(); }

			DXRenderObjectPool RenderObjectPool = DXRenderObjectPool(this);

			void SetShaders(std::string resource_name, DXShader* shader);
			void SetRenderMesh(std::string resource_name, DXRenderMesh* renderMesh);
			void SetVertices(std::string resource_name, const std::vector<SIMPLE_VERTEX>& vertices);
			void SetConstantBuffer(std::string resource_name, CONSTANT_BUFFER_DESC constantBuffer);

			DXShader* GetShaders(std::string resource_name) { return shaders[resource_name]; }
			DXRenderMesh* GetRenderMesh(std::string resource_name);
			// resource_name : typeid(cpu_buffer_struct).raw_name()
			CONSTANT_BUFFER_DESC GetConstantBuffer(std::string resource_name) { return constantBuffers[resource_name]; }
			std::vector<SIMPLE_VERTEX> GetVertices(std::string resource_name);

			void Init();

		private:
			void InitRenderMesh();
			void InitShader();
			void InitConstantBuffer();

			void Release()
			{
				for (auto resource : shaders)
					DXShader::Release(&resource.second);
				for (auto resource : renderMeshes)
					DXRenderMesh::Release(&resource.second);
				for (auto resource : loadedRenderMeshes)
					DXRenderMesh::Release(&resource.second);
				for (auto resource : constantBuffers)
					delete resource.second.StartSlots;
				vertexInfo.clear();
				loadedVertexInfo.clear();
			}
		};
	}
}