#pragma once

namespace UEngine
{
	namespace DXRenderer
	{
		class DXResourceManager
		{
		private:
			UINT constantBufferID = 0;
			std::map<std::string, std::unordered_map<std::string, void*>> creationQueue, deletionQueue, resources;
			std::map<std::string, CONSTANT_BUFFER_DESC> cBufferPreset;

			void InitShaders();
			void InitMeshes();
			void InitCBuffers();

		public:
			std::string TYPE_VIEW = typeid(UEngine::DXRenderer::DXView).raw_name();
			std::string TYPE_TEXTURE = typeid(UEngine::DXRenderer::DXTexture).raw_name();
			std::string TYPE_SAMPLER_STATE = typeid(UEngine::DXRenderer::DXSamplerState).raw_name();
			std::string TYPE_SHADER = typeid(UEngine::DXRenderer::DXShader).raw_name();
			std::string TYPE_RENDERMESH = typeid(UEngine::DXRenderer::DXRenderMesh).raw_name();
			std::string TYPE_CONSTANT_BUFFER = typeid(UEngine::DXRenderer::DXConstantBuffer).raw_name();

			DXResourceManager() = default;
			~DXResourceManager() { Release(); }

			template <typename T>
			void AddResource(std::string name, T* resource);
			template <typename T>
			void RemoveResource(std::string name);
			template <typename T>
			T* GetResource(std::string name);

			const CONSTANT_BUFFER_DESC& GetCBufferPreset(std::string typeName) { return cBufferPreset[typeName]; }
			const std::string GetNextCBufferID() { return std::to_string(constantBufferID++); }

			void ApplyChange();
			void Init();
			void Release();
		};

		template<typename T>
		inline void DXResourceManager::AddResource(std::string name, T* resource)
		{
			std::string typeName = typeid(T).raw_name();

			if (!std::is_same<T, UEngine::DXRenderer::DXView>::value &&
				!std::is_same<T, UEngine::DXRenderer::DXTexture>::value &&
				!std::is_same<T, UEngine::DXRenderer::DXSamplerState>::value &&
				!std::is_same<T, UEngine::DXRenderer::DXShader>::value &&
				!std::is_same<T, UEngine::DXRenderer::DXRenderMesh>::value &&
				!std::is_same<T, UEngine::DXRenderer::DXConstantBuffer>::value)
				throw std::runtime_error("Invalid resource type");


			if (creationQueue[typeName].size() > 0 &&
				creationQueue[typeName].find(name) != creationQueue[typeName].end())
				throw std::runtime_error("A resource with the given name is already queued");
			creationQueue[typeName][name] = resource;
		}

		template<typename T>
		inline void DXResourceManager::RemoveResource(std::string name)
		{
			std::string typeName = typeid(T).raw_name();

			if (!std::is_same<T, UEngine::DXRenderer::DXView>::value &&
				!std::is_same<T, UEngine::DXRenderer::DXTexture>::value &&
				!std::is_same<T, UEngine::DXRenderer::DXSamplerState>::value &&
				!std::is_same<T, UEngine::DXRenderer::DXShader>::value &&
				!std::is_same<T, UEngine::DXRenderer::DXRenderMesh>::value &&
				!std::is_same<T, UEngine::DXRenderer::DXConstantBuffer>::value)
				throw std::runtime_error("Invalid resource type");

			if (deletionQueue[typeName].size() > 0 &&
				deletionQueue[typeName].find(name) != deletionQueue[typeName].end())
				throw std::runtime_error("A resource with the given name already queued");

			if (resources[typeName].size() == 0 ||
				resources[typeName].find(name) == resources[typeName].end())
				throw std::runtime_error("A resource with the given name doesn't exists");

			deletionQueue[typeName][name] = resources[typeName][name];
		}

		template<typename T>
		T* DXResourceManager::GetResource(std::string name)
		{
			std::string typeName = typeid(T).raw_name();

			if (typeName != TYPE_VIEW &&
				typeName != TYPE_TEXTURE &&
				typeName != TYPE_SAMPLER_STATE &&
				typeName != TYPE_SHADER &&
				typeName != TYPE_RENDERMESH &&
				typeName != TYPE_CONSTANT_BUFFER)
				throw std::runtime_error("A resource with the given type doesn't exists");

			if (resources[typeName].size() == 0 ||
				resources[typeName].find(name) == resources[typeName].end())
			{
				if (creationQueue[typeName].size() > 0 &&
					creationQueue[typeName].find(name) != creationQueue[typeName].end())
				{
					return static_cast<T*>(creationQueue[typeName][name]);
				}
				return nullptr;
			}
			else if (deletionQueue[typeName].size() > 0 &&
				deletionQueue[typeName].find(name) != deletionQueue[typeName].end())
			{
				return nullptr;
			}
			return static_cast<T*>(resources[typeName][name]);
		}
	}
}