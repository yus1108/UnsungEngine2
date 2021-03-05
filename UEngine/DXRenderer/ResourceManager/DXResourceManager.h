#pragma once
#include "DXSceneManager.h"
#include "DXViewManager.h"
#include "DXRenderObjectManager.h"

namespace UEngine
{
	namespace DXRenderer
	{
		class DXResourceManager
		{
			friend class DXRenderer;
		private:
			/*
				key: std::string - typeid(cpu_buffer_struct).raw_name()
				value: CONSTANT_BUFFER_DESC - description to create a buffer
			*/
			std::unordered_map<std::string, CONSTANT_BUFFER_DESC> constantBuffers;
			std::unordered_map<std::string, DXShader*> shaders;
			std::unordered_map<std::string, SHARED_RENDERMESH> defaultRenderMeshes;
			ResourceManager::DXRenderObjectManager renderObjectManager;
			ResourceManager::DXViewManager viewManager{ this };
			ResourceManager::DXSceneManager sceneManager{ this };

			DXResourceManager() {}
			~DXResourceManager() { Release(); }
		public:
			void Init();

		private:
			void InitRenderMesh();
			void InitShader();
			void InitConstantBuffer();

			void Release();
		};
	}
}

///*
		//	key: std::string - typeid(cpu_buffer_struct).raw_name()
		//	value: CONSTANT_BUFFER_DESC - description to create a buffer
		//*/
		//
		//std::unordered_map<std::string, DXShader*> shaders;
		//std::map<std::string, DXRenderMesh*> renderMeshes;
		//std::map<std::string, std::vector<SIMPLE_VERTEX>> vertexInfo;

		//std::unordered_map<std::string, SHARED_RENDERMESH*> shared_rendermesh;
		//std::unordered_map<std::string, DXScene*> scenes;